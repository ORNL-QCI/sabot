cmake_minimum_required(VERSION 2.8)
project(sabot)

################################################################################
# BUILD SETUP
################################################################################
option(USE_STATIC "Build with static libraries" OFF)
option(BUILD_RELEASE "Build for release" ON)
option(BUILD_TEST "Build unit tests" ON)

################################################################################
# SOURCES
################################################################################
set(sabot_sources
	randnum/generator.cpp
	language/dialect.cpp
	language/dialects/chpext.cpp
	language/interpreter.cpp
	state/state_type.cpp
	state/chp_state.cpp
	actions.cpp
	kernel.cpp
	net/server.cpp
	universe.cpp)

file(GLOB_RECURSE test_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test/*.cpp)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

################################################################################
# CONFIGURE BUILD TYPE
################################################################################
message("------------------------------")
message("Build Configuration:")

if(CMAKE_COMPILER_IS_GNUCC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
else()
    message(WARNING "Compiler is not GNUCC, using default warning levels.")
endif()

if(BUILD_RELEASE)
	message("Building for release")
	set(CMAKE_BUILD_TYPE Release)
	
	if(CMAKE_COMPILER_IS_GNUCC)
		# GNUCC doesn't automatically strip binaries
		message("Stripping binaries")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s")
		set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -s")
	else()
		message(WARNING "Compiler is not GNUCC, binaries not stripped.")
	endif()
else()
	message("Building for debug")
	set(CMAKE_BUILD_TYPE Debug)
endif()

set(required_boost_components system thread program_options)

if(BUILD_TEST)
	message("Building unit tests")
	list(APPEND required_boost_components unit_test_framework)
endif()

if(USE_STATIC)
	message("Using static libaries")
	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_USE_STATIC_RUNTIME ON)
	message(WARNING "Linking ZMQ dynamically.")
	find_library(ZMQ_LIB zmq REQUIRED)
else()
	message("Using shared libraries")
	set(Boost_USE_STATIC_LIBS OFF)
	set(Boost_USE_STATIC_RUNTIME OFF)
	find_library(ZMQ_LIB zmq REQUIRED)
	
	add_definitions(-DBOOST_TEST_DYN_LINK)
endif()

find_package(Boost COMPONENTS ${required_boost_components} REQUIRED)
link_directories(${Boost_LIBRARY_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/tpls")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/tpls/rapidjson/include/")

message("------------------------------")

################################################################################
# BUILD
################################################################################
add_library(sabot-core STATIC OBJECT ${sabot_sources})
add_executable(sabot $<TARGET_OBJECTS:sabot-core> "sabot.cpp")
target_link_libraries(sabot ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES}
						${ZMQ_LIB})

if(CMAKE_MAJOR_VERSION GREATER 2 AND CMAKE_MINOR_VERSION GREATER 0)
	target_compile_features(sabot-core PRIVATE cxx_range_for)
	target_compile_features(sabot PRIVATE cxx_range_for)
else()
	if(CMAKE_COMPILER_IS_GNUCXX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	else()
		message(WARNING "C++11 not explicitly set in compiler.")
	endif()
endif()

################################################################################
# TEST
################################################################################
if(BUILD_TEST)
	enable_testing()
	# Add alias make check for ctest
	add_custom_target(check COMMAND ctest)
	add_executable(sabot-test $<TARGET_OBJECTS:sabot-core>
					${CMAKE_CURRENT_SOURCE_DIR}/test/sabot.cpp ${test_sources})
	
	if(CMAKE_MAJOR_VERSION GREATER 2 AND CMAKE_MINOR_VERSION GREATER 0)
		target_compile_features(sabot-test PRIVATE cxx_range_for)
	endif()
	
	target_link_libraries(sabot-test pthread ${Boost_LIBRARIES} ${ZMQ_LIB})
	set_target_properties(sabot-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY
							${CMAKE_BINARY_DIR}/test)
	foreach(test ${test_sources})
		get_filename_component(name ${test} NAME_WE)
		if(NOT "sabot" STREQUAL ${name})
			add_test(NAME ${name} WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test
					COMMAND ${CMAKE_BINARY_DIR}/test/sabot-test
					--run_test=${name} --random=1)
		endif()
	endforeach(test)
endif()

################################################################################
# INSTALL
################################################################################
install(TARGETS sabot DESTINATION bin)

################################################################################
# UNINSTALL
################################################################################
#add_custom_target(uninstall COMMAND ${CMAKE_COMMAND}
#					-P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake)
