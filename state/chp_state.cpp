#include "chp_state.hpp"

namespace state {
	/**
	 * \brief state_type name.
	 */
	const char* chp_state::name = "chp_state";
	
	/**
	 * \brief Register state_type.
	 */
	const state_type_register<chp_state> chp_state::child_register(name);
	
	/**
	 * \brief Constructor.
	 */
	chp_state::chp_state() : quantum_state(name) {
	}
	
	/**
	 * \brief Destructor.
	 */
	chp_state::~chp_state() {
	}
	
	/**
	 * \brief Encode a state via a program.
	 */
	_chp_state chp_state::encode_state(const ::language::program& program) {
		_chp_state returnState;
		
		run_program(returnState, program);
		
		return returnState;
	}
	
	/**
	 * \brief Modify a state via a program.
	 */
	bool chp_state::modify_state(_chp_state& state, const ::language::program& program) {
		return run_program(state, program);
	}
	
	/**
	 * \brief Measure a state via a program.
	 */
	bool chp_state::measure_state(_chp_state& state, const ::language::program& program, string_buffer<>& buffer) {
		return run_program(state, program, buffer);
	}
	
	/**
	 * \brief Compute the result of a program by temporarily storing state.
	 */
	bool chp_state::compute_result(const ::language::program& program, string_buffer<>& buffer) {
		_chp_state tempState;
		
		return run_program(tempState, program, buffer);
	}
}
