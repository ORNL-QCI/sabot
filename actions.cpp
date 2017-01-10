#include "actions.hpp"
#include <limits>
#include "universe.hpp"

namespace actions {
	registered_calls registeredCalls = {{0}, {0}, {0}};
	
	void register_call(call item) {
		std::size_t id = 0;
		while(id < ACTIONS_MAXIMUM
				&& registeredCalls.methods[id*ACTIONS_METHOD_MAXIMUM] != 0) {
			id++;
		}
		
		// We can only register a certain number of actions
		if(id >= ACTIONS_MAXIMUM) {
			throw std::logic_error(err_msg::_arybnds);
		}
		
		// We need a method name for the action...
		if(item.methodSize == 0) {
			throw std::logic_error(err_msg::_zrlngth);
		}
		
		// ...but this method name needs to fit in our storage
		if(item.methodSize > ACTIONS_METHOD_MAXIMUM) {
			throw std::logic_error(err_msg::_arybnds);
		}
		
		#ifdef SABOT_THROW
		if(UNLIKELY(item.parameterCount > 31)) {
			throw std::invalid_argument(err_msg::_arybnds);
		}
		#endif
		
		registeredCalls.methodsAttr[id] = item.methodSize + (item.parameterCount << 5);
		memcpy(&registeredCalls.methods[id*ACTIONS_METHOD_MAXIMUM],
				item.method,
				item.methodSize);
		registeredCalls.functions[id] = item.function;
	}
	
	net::response* find_call(const net::request& request) {
		static_assert(std::numeric_limits<SABOT_QUINTN>::max() >= ACTIONS_MAXIMUM,
				"Too many actions for current types below.");
		
		const auto methodAttr = request.method_attr();
		for(SABOT_QUINTN i = 0; i < ACTIONS_MAXIMUM; i+=8) {
			#ifndef SABOT_64BIT
			#	error "Sorry, you have to vectorize this for 32 bits."
			#endif
			// This is vectorized: take a look at 8 actions per iteration of outer loop
			uint64_t methodsAttr = 0;
			memcpy(&methodsAttr, &registeredCalls.methodsAttr[i], 8);
			// No, moving this to another function would not make it more readable
			for(SABOT_QUINTN j = 0; j < 8; j++) {
				// This inner loop checks each of the individual 8 actions retrieved by
				// the outer loop
				const SABOT_QUINTN wrdOffset = j*8;
				const SABOT_QUINTN strOffset = (i+j)*ACTIONS_METHOD_MAXIMUM;
				// We don't need to mask the LSB because we know once it is 0, any
				// remaining methods are null and the more significant bytes are 0 too.
				if(UNLIKELY((methodsAttr >> wrdOffset) == 0)) {
					// We've reached the end of the registered actions
					
					// This inner loop cannot easily be split into another function, so
					// the goto statement stays.
					goto NOTFOUND;
				} else if(
							((methodsAttr >> wrdOffset) & 0xFF) == methodAttr
							&& memcmp(request.method(),
								&registeredCalls.methods[strOffset],
								methodAttr & 0x1F) == 0) {
					return registeredCalls.functions[i+j](request);
				}
			}
		}
		
	 NOTFOUND:
		throw std::runtime_error(err_msg::_tpntfnd);
	}
	
	namespace get_uniform_integer {
		const action_registrar self_register(
				call {
					"get_uniform_integer",
					19,
					3,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			const auto count = request.parameter<std::size_t>(0);
			auto result = new unsigned long int[count];
			
			for(std::size_t i = 0; i < count; i++) {
				// Perform required work
				result[i] = ::universe::here().random_generator().get_uniform_integer(
						request.parameter<std::size_t>(1),
						request.parameter<std::size_t>(2));
			}
			
			// Return with a response
			// Ownership of result is transferred into response.
			return new net::response(result, count);
		}
	}
	
	namespace get_uniform_real {
		const action_registrar self_register(
				call {
					"get_uniform_real",
					16,
					3,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			const auto count = request.parameter<std::size_t>(0);
			auto result = new double[count];
			
			for(std::size_t i = 0; i < count; i++) {
				// Perform required work
				result[i] = ::universe::here().random_generator().get_uniform_real(
						request.parameter<double>(1),
						request.parameter<double>(2));
			}
			
			// Return with a response
			// Ownership of result is transferred into response.
			return new net::response(result, count);
		}
	}
	
	namespace get_weighted_integer {
		const action_registrar self_register(
				call {
					"get_weighted_integer",
					20,
					3,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			const auto count = request.parameter<std::size_t>(0);
			const auto size = request.parameter<std::size_t>(1);
			const auto weights = request.parameter<double*>(2);
			auto result = new unsigned long int[count];
			
			for(std::size_t i = 0; i < count; i++) {
				// Perform required work
				result[i] = ::universe::here().random_generator().get_weighted_integer(
						weights,
						size);
			}
			
			delete[] weights;
			
			// Return with a response
			// Ownership of result is transferred into response.
			return new net::response(result, count);
		}
	}
	
	namespace create_kernel {
		const action_registrar self_register(
				call {
					"create_kernel",
					13,
					0,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			UNUSED(request);
			
			// Perform required work
			const kernel::id_t result = ::universe::here().create_kernel();
			
			// Return with a response
			return new ::net::response(result);
		}
	}
	
	namespace delete_kernel {
		const action_registrar self_register(
				call {
					"delete_kernel",
					13,
					1,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			const bool result = ::universe::here().delete_kernel(
					request.parameter<kernel::id_t>(0));
			
			// Return with a response
			return new ::net::response(result);
		}
	}
	
	namespace compile_macro {
		const action_registrar self_register(
				call {
					"compile_macro",
					13,
					4,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			const std::uint64_t result = ::universe::here().compile_macro(
					request.parameter<unsigned long int>(0),
					request.parameter<const char*>(1),
					request.parameter<const char*>(2),
					request.parameter<unsigned long int>(3));
			
			// Return with a response
			return new ::net::response(result);
		}
	}
	
	namespace create_system {
		const action_registrar self_register(
				call {
					"create_system",
					13,
					1,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			const auto systemId = ::universe::here().create_system(
					request.parameter<const char*>(0));
			
			// Return with a response
			return new net::response(systemId);
		}
	}
	
	namespace delete_system {
		const action_registrar self_register(
				call {
					"delete_system",
					13,
					1,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			const bool result = ::universe::here().delete_system(
					request.parameter<quantum_system::id_t>(0));
			
			// Return with a response
			return new net::response(result);
		}
	}
	
	namespace create_state {
		const action_registrar self_register(
				call {
					"create_state",
					12,
					4,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			const std::uint_fast64_t result = ::universe::here().create_state(
					request.parameter<unsigned long int>(0),
					request.parameter<const char*>(1),
					request.parameter<const char*>(2),
					request.parameter<unsigned long int>(3));
			
			// Return with a response
			return new ::net::response(result);
		}
	}
	
	namespace delete_state {
		const action_registrar self_register(
				call {
					"delete_state",
					12,
					2,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			const bool result = ::universe::here().delete_state(
					request.parameter<unsigned long int>(0),
					request.parameter<unsigned long int>(1));
			
			// Return with a response
			return new ::net::response(result);
		}
	}
	
	namespace modify_state {
		const action_registrar self_register(
				call {
					"modify_state",
					12,
					5,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			const bool result = ::universe::here().modify_state(
					request.parameter<unsigned long int>(0),
					request.parameter<unsigned long int>(1),
					request.parameter<const char*>(2),
					request.parameter<const char*>(3),
					request.parameter<unsigned long int>(4));
			
			// Return with a response
			return new ::net::response(result);
		}
	}
	
	namespace measure_state {
		const action_registrar self_register(
				call {
					"measure_state",
					13,
					5,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			string_buffer<> buffer;
			
			const bool result = ::universe::here().measure_state(
					request.parameter<unsigned long int>(0),
					request.parameter<unsigned long int>(1),
					request.parameter<const char*>(2),
					request.parameter<const char*>(3),
					request.parameter<unsigned long int>(4),
					buffer);
			
			if(UNLIKELY(!result)) {
				throw std::runtime_error("measure state returned false");
			}
			
			// Return with a response
			return new ::net::response(std::move(buffer));
		}
	}
	
	namespace compute_result {
		const action_registrar self_register(
				call {
					"compute_result",
					14,
					4,
					process
				}
			);
		
		net::response* process(const net::request& request) {
			// Perform required work
			string_buffer<> buffer;
			
			const bool result = ::universe::here().compute_result(
					request.parameter<unsigned long int>(0),
					request.parameter<const char*>(1),
					request.parameter<const char*>(2),
					request.parameter<unsigned long int>(3),
					buffer);
			
			if(UNLIKELY(!result)) {
				throw std::runtime_error("compute result returned false");
			}
			
			// Return with a response
			return new ::net::response(std::move(buffer));
		}
	}
}