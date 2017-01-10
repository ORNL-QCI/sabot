#ifndef _QUANTUM_SYSTEM_HPP
#define _QUANTUM_SYSTEM_HPP

#include <common.hpp>
#include "state/state_type.hpp"

/**
 * \brief A discrete quantum system.
 * 
 * A system has a state type object and a noise type object which holds relevent
 * parameters. We use move semantics.
 */
struct quantum_system {
 public:
	using id_t = std::uint_fast64_t;
	
	/**
	 * \brief Initialization constructor.
	 */
	quantum_system(const char* stateType)
			: stateType(state::state_type_factory::instantiate(stateType)) {
	}
	
	/**
	 * \brief Copy constructor disabled.
	 */
	quantum_system(const quantum_system&) = delete;
	
	/**
	 * \brief Move constructor.
	 */
	quantum_system(quantum_system&& system) {
		stateType = system.stateType;
		system.stateType = 0;
	}
	
	/**
	 * \brief Assignment operator disabled.
	 */
	quantum_system& operator=(const quantum_system&) = delete;
	
	/**
	 * \brief Move assignment operator.
	 */
	quantum_system& operator=(quantum_system&& system) {
		std::swap(stateType, system.stateType);
		return *this;
	}
	
	/**
	 * \brief Destructor.
	 */
	~quantum_system() {
		if(stateType != 0) {
			delete stateType;
		}
	}
	
	/**
	 * \brief Get a pointer to the state type object.
	 */
	inline state::state_type& state_type() const {
		return *stateType;
	}
	

 private:
	/**
	 * \brief The type of state the quantum channel can handle.
	 * 
	 * This also contains the list of states, since the type of element is dynamic.
	 */
	state::state_type* stateType;
};

#endif
