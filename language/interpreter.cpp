#include "interpreter.hpp"
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

namespace language {
	/**
	 * \brief Parse a single string consisting of many instructions seperated
	 * by the specified line delimiter character into a single program. This will
	 * seperate instructions into functions as needed and place all other instructions
	 * in the program as well.
	 */
	program interpreter::parse_program(const char* const in, const char lineDelimiter) const {
		const char* pos = remove_leading_whitespace(in);
		instruction* instructionList(0);
		std::size_t instructionCount(0);
		
		while(*pos != '\0') {
			const char* opcodeStart = pos;
			// Find the end of the opcode
			while(*pos != _dialect->opcode_delimiter() && *pos != '\0') {
				pos++;
			}
			const auto& opcode = _dialect->find_idef(opcodeStart, pos-opcodeStart);
			
			std::uint_fast64_t operandsRemaining = opcode.operandCount;
			machine::operand_t operands[machine::max_operand_count];
			
			// Find each operand
			while(operandsRemaining > 0) {
				/** \todo: handle whitespace */
				const char* operandStart = ++pos;
				while(*pos != _dialect->operand_delimiter()
						&& *pos != lineDelimiter
						&& *pos != '\0') {
					pos++;
				}
				
				operands[opcode.operandCount - operandsRemaining] = utility::stuif(operandStart, pos-operandStart);
				operandsRemaining--;
				operandStart = pos+1;
			}
			if(instructionCount > 0) {
				instruction* temp = instructionList;
				++instructionCount;
				instructionList = new instruction[instructionCount];
				memcpy(instructionList, temp, (instructionCount-1)*sizeof(instruction));
				delete[] temp;
			} else {
				instructionList = new instruction[1];
				++instructionCount;
			}
			instructionList[instructionCount-1] = {opcode.bytecode, {}};
			memcpy(&instructionList[instructionCount-1].operands, operands, instruction_definition::operand_array_size);
			if(*pos != '\0') {
				pos++;
			}
		}
		
		return program(std::move(instructionList), instructionCount);
	}
}
