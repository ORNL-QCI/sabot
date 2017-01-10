#ifndef _LANGUAGE_INSTRUCTION_HPP
#define _LANGUAGE_INSTRUCTION_HPP

#include <common.hpp>
#include "machine.hpp"

namespace language {
	/**
	 * \brief A machine instruction that has been interpreted and converted into bytecode
	 * and has validated operands.
	 * 
	 * \warning This must be constructable by a list initialization.
	 */
	struct instruction {
	 public:
		/**
		 * \brief The bytecode of the instruction, referring to a unique operation.
		 */
		machine::bytecode_t bytecode;
		
		/**
		 * \brief The operands required by the operation the bytecode refers to.
		 */
		machine::operand_t operands[machine::max_operand_count];
	};
}

#endif
