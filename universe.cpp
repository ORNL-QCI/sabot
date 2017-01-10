#include "universe.hpp"

universe& universe::here() {
	static universe instance;
	return instance;
}

universe::universe() {
}

universe::~universe() {
	
}

const language::interpreter& universe::interpreter(const char* const dialect,
		std::mutex*& elementMutex, boost::shared_mutex*& listMutex) {
	try {
		return interpreters.find(dialect, elementMutex, listMutex);
	} catch(const std::out_of_range& e) {
		// Not found, so create one
		return interpreters.insert_and_return(language::interpreter(dialect),
				elementMutex, listMutex, dialect);
	}
}

kernel& universe::get_kernel(kernel::id_t kernelId,
		std::mutex*& elementMutex,
		boost::shared_mutex*& listMutex) {
	return kernels.edit(kernelId, elementMutex, listMutex);
}

language::program universe::compile_program(const char* const dialect, 
			const char* const data,
			const char lineDelimiter) {
	std::mutex* interpreterElementMutex = 0;
	boost::shared_mutex* interpreterListMutex = 0;
	const auto& interp = interpreter(dialect, interpreterElementMutex, interpreterListMutex);
	
	std::lock_guard<std::mutex> interpreterElementLock(*interpreterElementMutex);
	boost::upgrade_lock<boost::shared_mutex> dialectListLock(*interpreterListMutex);
	
	return interp.parse_program(data, lineDelimiter);
}

std::uint64_t universe::compile_macro(const kernel::id_t kernelId,
		const char* const dialect,
		const char* const data,
		const char lineDelimiter) {
	auto result = compile_program(dialect, data, lineDelimiter);
	
	std::mutex* kernelElementMutex = 0;
	boost::shared_mutex* kernelListMutex = 0;
	auto& ckernel = get_kernel(kernelId, kernelElementMutex, kernelListMutex);
	
	std::lock_guard<std::mutex> kernelElementLock(*kernelElementMutex);
	boost::upgrade_lock<boost::shared_mutex> kernelListLock(*kernelListMutex);
	
	return ckernel.insert_macro(std::move(result));
}

const language::program& universe::get_macro(const kernel::id_t kernelId,
		const std::uint64_t macroId) {
	std::mutex* kernelElementMutex = 0;
	boost::shared_mutex* kernelListMutex = 0;
	auto& ckernel = get_kernel(kernelId, kernelElementMutex, kernelListMutex);
	
	std::lock_guard<std::mutex> kernelElementLock(*kernelElementMutex);
	boost::upgrade_lock<boost::shared_mutex> kernelListLock(*kernelListMutex);
	
	return ckernel.get_macro(macroId);
}

kernel::id_t universe::create_kernel() {
	return kernels.insert(kernel());
}

bool universe::delete_kernel(const kernel::id_t kernelId) {
	return kernels.erase(kernelId);
}

quantum_system::id_t universe::create_system(const char* stateType) {
	return quantumSystems.insert(quantum_system(stateType));
}

bool universe::delete_system(const quantum_system::id_t systemId) {
	return quantumSystems.erase(systemId);
}
	
std::uint_fast64_t universe::create_state(const quantum_system::id_t systemId,
		const char* const dialect,
		const char* const data,
		const char lineDelimiter) {
	try {
		// Return the state id
		std::mutex* elementMutex = 0;
		boost::shared_mutex* listMutex = 0;
		const auto& system = quantumSystems.get(systemId, elementMutex, listMutex);
		
		std::lock_guard<std::mutex> elementLock(*elementMutex);
		boost::upgrade_lock<boost::shared_mutex> listLock(*listMutex);
		
		return system.state_type().program_insert_state(
				compile_program(dialect, data, lineDelimiter));
	} catch(const std::out_of_range& e) {
		// System was not found
		throw std::exception();
	}
}

bool universe::modify_state(const quantum_system::id_t systemId,
		const std::uint_fast64_t stateId,
		const char* const dialect, 
		const char* const data,
		const char lineDelimiter) {
	try {
		std::mutex* elementMutex = 0;
		boost::shared_mutex* listMutex = 0;
		const auto& system = quantumSystems.get(systemId, elementMutex, listMutex);
		
		std::lock_guard<std::mutex> elementLock(*elementMutex);
		boost::upgrade_lock<boost::shared_mutex> listLock(*listMutex);
		
		return system.state_type().program_modify_state(stateId,
				compile_program(dialect, data, lineDelimiter));
	} catch(const std::out_of_range& e) {
		// System was not found
		return false;
	}
}

bool universe::measure_state(const quantum_system::id_t systemId,
		const std::uint_fast64_t stateId,
		const char* const dialect, 
		const char* const data,
		const char lineDelimiter,
		string_buffer<>& buffer) {
	try {
		std::mutex* elementMutex = 0;
		boost::shared_mutex* listMutex = 0;
		const auto& system = quantumSystems.get(systemId, elementMutex, listMutex);
		
		std::lock_guard<std::mutex> elementLock(*elementMutex);
		boost::upgrade_lock<boost::shared_mutex> listLock(*listMutex);
		
		return system.state_type().program_measure_state(stateId, 
				compile_program(dialect, data, lineDelimiter), buffer);
	} catch(const std::out_of_range& e) {
		// System was not found
		return false;
	}
}

bool universe::delete_state(const quantum_system::id_t systemId,
		const std::uint_fast64_t stateId) {
	try {
		std::mutex* elementMutex = 0;
		boost::shared_mutex* listMutex = 0;
		const auto& system = quantumSystems.get(systemId, elementMutex, listMutex);
		
		std::lock_guard<std::mutex> elementLock(*elementMutex);
		boost::upgrade_lock<boost::shared_mutex> listLock(*listMutex);
		
		return system.state_type().delete_state_element(stateId);
	} catch(const std::out_of_range& e) {
		// System was not found
		return false;
	}
}

bool universe::compute_result(const quantum_system::id_t systemId,
		const char* const dialect, 
		const char* const data,
		const char lineDelimiter,
		string_buffer<>& buffer) {
	try {
		std::mutex* elementMutex = 0;
		boost::shared_mutex* listMutex = 0;
		const auto& system = quantumSystems.get(systemId, elementMutex, listMutex);
		
		std::lock_guard<std::mutex> elementLock(*elementMutex);
		boost::upgrade_lock<boost::shared_mutex> listLock(*listMutex);
		
		return system.state_type().program_compute_result(compile_program(dialect, data, lineDelimiter), buffer);
	} catch(const std::out_of_range& e) {
		// System was not found
		return false;
	}
}