#include "chpext.hpp"

namespace language {
	namespace dialects {
		const dialect_register<chpext> chpext::name("chpext");
		
		const char chpext::opcodeDelimiter = ' ';
		
		const char chpext::operandDelimiter = ',';
		
		INSDEF_LISTI_HELPER(chpext, instructionDefs) = {
			// We map a string mnemonic to a machine operation
			INSDEF_HELPER("macro", macro),
			INSDEF_HELPER("init", initialize),
			INSDEF_HELPER("i", identity),
			INSDEF_HELPER("h", hadamard),
			INSDEF_HELPER("p", phase),
			INSDEF_HELPER("m", measure),
			INSDEF_HELPER("c", cnot),
			INSDEF_HELPER("x", paulix),
			INSDEF_HELPER("y", pauliy),
			INSDEF_HELPER("z", pauliz),
		};	
		
		chpext::chpext() : dialect(opcodeDelimiter, operandDelimiter) {
			constexpr const auto arraySize = sizeof(instructionDefs);
			constexpr const auto elementSize = sizeof(instructionDefs[0]);
			
			register_instructions(instructionDefs, arraySize/elementSize);
		}
	}
}
