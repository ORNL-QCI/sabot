#ifndef _STATE_QUANTUM_STATE_HPP
#define _STATE_QUANTUM_STATE_HPP

#include <common.hpp>
#include "state_type.hpp"
#include "../ms_container.hpp"

namespace state {
	/**
	 * \brief The actual parent class for all implemented state types.
	 */
	template <class T> class quantum_state : public state_type {
	 public:
		using state_container_type = ::ms_container<std::uint_fast64_t, T, 8>;
		
		/**
		 * \brief Constructor.
		 */
		quantum_state(const char* const name) : state_type(name) {
		}
		
		/**
		 * \brief Get the number of states.
		 */
		inline std::uint_fast64_t get_state_count() {
			return states.size();
		}
		
		/**
		 * \brief Insert a state with a given program.
		 */
		std::uint_fast64_t program_insert_state(const ::language::program&& program) {
			return states.insert(encode_state(program));
		}
		
		/**
		 * \brief Modify a state with a given program.
		 */
		bool program_modify_state(const std::uint_fast64_t stateId, const ::language::program&& program) {
			std::mutex* elementMutex = 0;
			boost::shared_mutex* editMutex = 0;
			
			auto& temp = states.edit(stateId, elementMutex, editMutex);
			
			assert(elementMutex != 0);
			assert(editMutex != 0);
			
			std::lock_guard<std::mutex> elementLock(*elementMutex);
			boost::unique_lock<boost::shared_mutex> editULock(*editMutex);
			
			return modify_state(temp, program);
		}
		
		/**
		 * \brief Measure a state with a given program.
		 */
		bool program_measure_state(const std::uint_fast64_t stateId, const ::language::program&& program, string_buffer<>& buffer) {
			std::mutex* elementMutex = 0;
			boost::shared_mutex* editMutex = 0;
			
			/** \todo this probably needs to be reduced */
			try {
				auto& temp = states.edit(stateId, elementMutex, editMutex);
				
				assert(elementMutex != 0);
				assert(editMutex != 0);
				
				std::lock_guard<std::mutex> elementLock(*elementMutex);
				boost::unique_lock<boost::shared_mutex> editULock(*editMutex);
				
				return measure_state(temp, program, buffer);
			} catch (const std::out_of_range& e) {
				return false;
			}
		}
		
		/**
		 * \brief Compute the result of a program with storing a state.
		 */
		bool program_compute_result(const ::language::program&& program, string_buffer<>& buffer) {
			try {
				return compute_result(program, buffer);
			} catch (const std::out_of_range& e) {
				return false;
			}
		}
		
		/**
		 * \brief Delete a state.
		 */
		inline bool delete_state_element(const std::uint_fast64_t stateId) {
			return states.erase(stateId);
		}
		
		/**
		 * \brief Return a state in it's native type.
		 * 
		 * Since this cannot be accessed through the parent, this is used
		 * only when the child type is known.
		 */
		inline const T& get_encoded_state(const std::uint_fast64_t stateId) const {
			return states.get(stateId);
		}
	
	 protected:
		virtual T encode_state(const ::language::program& program) = 0;
		virtual bool modify_state(T& state, const ::language::program& program) = 0;
		virtual bool measure_state(T& state, const ::language::program& program , string_buffer<>& buffer) = 0;
		virtual bool compute_result(const ::language::program& program, string_buffer<>& buffer) = 0;
	
	 private:
		state_container_type states;
	};
}

#endif
