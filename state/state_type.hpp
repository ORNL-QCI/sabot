#ifndef _STATE_STATE_TYPE_HPP
#define _STATE_STATE_TYPE_HPP

#include <common.hpp>
#include "../string_buffer.hpp"
#include "../language/program.hpp"
#include <map>

namespace state {
	/**
	 * \brief This class is the base for all types of quantum states.
	 *
	 * The only child of this class should be quantum_state as this
	 * is a wrapper for that template class. All implementations of
	 * states should inherit from quantum_state.
	 */
	class state_type {
	 public:
		/**
		 * \brief Constructor.
		 */
		state_type(const char* const name) : name(name) {
		}
		
		/**
		 * \brief Copy constructor is disabled.
		 */
		state_type(const state_type&) = delete;
		
		/**
		 * \brief Move constructor is disabled.
		 */
		state_type(state_type&&) = delete;
		
		/**
		 * \brief Assignment operator is disabled.
		 */
		state_type& operator=(const state_type&) = delete;
		
		/**
		 * \brief Move assignment operator is disabled.
		 */
		state_type& operator=(state_type&) = delete;
		
		/**
		 * \brief Destructor.
		 */
		virtual ~state_type() {
		}
		
		/**
		 * \brief Return the name of the state_type.
		 */
		inline const char* get_name() const {
			return name;
		}
		
		// Implemented in derived quantum_state
		virtual std::uint_fast64_t get_state_count() = 0;
		virtual std::uint_fast64_t program_insert_state(const ::language::program&& program) = 0;
		virtual bool program_modify_state(const std::uint_fast64_t stateId, const ::language::program&& program) = 0;
		virtual bool program_measure_state(const std::uint_fast64_t stateId, const ::language::program&& program, string_buffer<>& buffer) = 0;
		virtual bool delete_state_element(const std::uint_fast64_t stateId) = 0;
		virtual bool program_compute_result(const ::language::program&& program, string_buffer<>& buffer) = 0;
	
	 private:
		const char* name;
	};
	
	/**
	 * \brief Create a new state_type from the template parameter.
	 */
	template<typename T> state_type* create_state_type() {
		return new T;
	}
	
	/**
	 * \brief Factory to create and keep track of state_types.
	 */
	struct state_type_factory {
	 public:
		using state_type_map_type = std::map<const char*, state_type*(*)()>;
		
		/**
		 * \brief Instantiate a state_type from its name.
		 */
		static state_type* instantiate(const char* const name) {
			for(auto it : state_type_map()) {
				if(strcmp(it.first, name) == 0) {
					return it.second();
				}
			}
			throw std::invalid_argument("name not found.");
		}
		
	 protected:
		/**
		 * \brief Return the map of state_types.
		 */
		static state_type_map_type& state_type_map() {
			return _state_type_map;
		}
	
	 private:
		static state_type_map_type _state_type_map;
	};
	
	/**
	 * \brief Template to register a state_type in the state_type factory.
	 * The template parameter is the state_type type.
	 */
	template<typename T> struct state_type_register : state_type_factory {
	 public:
		/**
		 * \brief Constructor takes a name of the state_type to be
		 * referenced when instantiating later.
		 */
		state_type_register(const char* const name) {
			auto it(state_type_map().insert(std::make_pair(name, &create_state_type<T>)));
			assert(it.second == true);
		}
	};
}

#endif
