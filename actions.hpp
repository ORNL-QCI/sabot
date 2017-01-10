#ifndef _ACTIONS_HPP
#define _ACTIONS_HPP

#include <common.hpp>
#include "net/request.hpp"
#include "net/response.hpp"

/**
 * \brief The maximum number of actions that can be registered.
 * 
 * \note This should should be chosen such that the registered_calls struct fits nicely
 * into memory and into cache lines, see documentation for that struct for more
 * information.
 */
#define ACTIONS_MAXIMUM 16

/**
 * \brief The maximum number of bytes for an action method name.
 * 
 * \note bytes.
 * 
 * \note This should should be chosen such that the registered_calls struct fits nicely
 * into memory and into cache lines, see documentation for that struct for more
 * information.
 *
 * \note This does not include the null terminator.
 */
#define ACTIONS_METHOD_MAXIMUM 30

namespace actions {
	/**
	 * \brief Function pointer to the processing function associated with a call.
	 */
	using fctr_ptr_t = net::response*(*)(const net::request&);
	
	/**
	 * \brief A call to be registered.
	 * 
	 * The method string should not be null-terminated. The callFunctor requires the
	 * function given to be global.
	 */
	struct call {
		char method[ACTIONS_METHOD_MAXIMUM];
		unsigned char methodSize;
		unsigned char parameterCount;
		fctr_ptr_t function;
	};
	
	/**
	 * \brief The calls that have been registered at runtime launch.
	 * 
	 * With the current sizes, this struct is 640 bytes in size and on most processors is
	 * contained within 10 cache lines. The hope is that this entire structure will be
	 * in very quick memory since the entire struct is accessed frequently.
	 */
	extern struct registered_calls {
		fctr_ptr_t functions[ACTIONS_MAXIMUM];
		unsigned char methodsAttr[ACTIONS_MAXIMUM];
		char methods[ACTIONS_MAXIMUM*(ACTIONS_METHOD_MAXIMUM+1)];
	} registeredCalls;
	
	/**
	 * \brief Register a call with the registered_calls container..
	 */
	UNROLL_LOOP
	void register_call(call item);
	
	/**
	 * \brief This is the base class for processing functions that are called by clients.
	 */
 	struct action_registrar {
	 public:
		/**
		 * \brief Constructor registers the processing function.
		 */
		action_registrar(call item) {
			register_call(item);
		}
		
		/**
		 * \brief Copy constructor is disabled.
		 */
		action_registrar(const action_registrar&) = delete;
		
		/**
		 * \brief Move constructor is disabled.
		 */
		action_registrar(action_registrar&&) = delete;
		
		/**
		 * \brief Assignment operator is disabled.
		 */
		action_registrar& operator=(const action_registrar&) = delete; 
		
		/**
		 * \brief Move assignment operator is disabled.
		 */
		action_registrar& operator=(action_registrar&&) = delete;
		
		/**
		 * \brief Empty destructor.
		 */
		~action_registrar() {
		}
	};
	
	/**
	 * \brief Find a registered call, execute the associated processing function with the
	 * supplied request, and return a response.
	 * 
	 * We return a pointer to a response here and lose ownership of it upon return. This
	 * is so we don't recopy this response upon transmission to the client. See the server
	 * for details on this.
	 */
	UNROLL_LOOP
	net::response* find_call(const net::request& request);
	
	/**
	 * \brief Get a certain number of uniformly distributed random integers.
	 */
	namespace get_uniform_integer {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief Get a certain number of uniformly distributed random reals.
	 */
	namespace get_uniform_real {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief \todo
	 */
	namespace get_weighted_integer {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief Create a kernel with some parameters.
	 */
	namespace create_kernel {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& a);
	}
	
	/**
	 * \brief Delete a kernel by its id.
	 */
	namespace delete_kernel {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& a);
	}
	
	/**
	 * \brief \todo
	 */
	namespace compile_macro {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& a);
	}
	
	/**
	 * \brief Create a system with some parameters.
	 */
	namespace create_system {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief Delete a system by its id.
	 */
	namespace delete_system {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief Create a state within a system with a given circuit description.
	 */
	namespace create_state {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief Delete a state within a system by its id.
	 */
	namespace delete_state {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief Modify a state with a given circuit description without returning results.
	 */
	namespace modify_state {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief Modify a state with a given circuit description and return measurement
	 * results.
	 */
	namespace measure_state {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
	
	/**
	 * \brief Create and measure a state with a given circuit description without storing
	 * it within a system.
	*/
	namespace compute_result {
		/**
		 * \brief Register the action with the processor.
		 */
		extern const action_registrar self_register;
		
		/**
		 * \brief Processing function.
		 */
		net::response* process(const net::request& request);
	}
}

#endif