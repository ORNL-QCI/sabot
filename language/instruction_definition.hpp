#ifndef _LANGUAGE_INSTRUCTION_DEFINITION_HPP
#define _LANGUAGE_INSTRUCTION_DEFINITION_HPP

#include <common.hpp>
#include <machine.hpp>

/**
 * \brief Helper macro to define a new list of instruction definitions.
 * 
 * Use this in your dialect header.
 */
#define INSDEF_LISTD_HELPER(name) static const instruction_definition name[]

/**
 * \brief Helper macro to initialize a new list of instruction definitions.
 * 
 * Use this in your dialect source file, with reference to the definition helper in the
 * header.
 */
#define INSDEF_LISTI_HELPER(cname, name) const instruction_definition cname::name[]

/**
 * \brief Helper macro to define an instruction definition with the dialect mnemonic and
 * the corresponding operation.
 */
#define INSDEF_HELPER(name, method) \
		instruction_definition { \
			name, \
			strlen(name), \
			MACHINE_OPERANDC_HELPER(method), \
			MACHINE_BYTECODE_HELPER(method) \
		}

namespace language {
	/**
	 * \brief A definition of a machine instruction, including all properties required for
	 * validating assembly code and compiling assembly into machine code.
	 * 
	 * Some of the nitty-gritty properties are defined in the machine description, such as
	 * storage types, but this is the type used when describing the programming of that
	 * machine in a dialect language.
	 * 
	 * \warning This must be constructable by a list initialization.
	 * 
	 * \warning We provide no means to check whether a dialect has defined two
	 * instructions with the same mnemonic. While it *is* valid to define two different
	 * mnemonics that point to the same operation, it is *not* valid to define two equal
	 * mnemonics that point to different operations.
	 */
	struct instruction_definition {
	 public:
		/**
		 * \brief The maximum number of bytes a mnemonic may be.
		 * 
		 * We do not store a mnemonic with a null terminator, so we may use up to this
		 * many characters in a mnemonic.
		 */
		constexpr static const SABOT_HUINTN max_mnemonic_len = sizeof(SABOT_UINTN);
		
		/**
		 * \brief The size of the operand array in bytes.
		 */
		constexpr static const SABOT_HUINTN operand_array_size =
				machine::max_operand_count * sizeof(SABOT_UINTN);
		
		/**
		 * \brief A textual mnemonic used in assembly code.
		 */
		const char mnemonic[max_mnemonic_len];
		
		/**
		 * \brief The number of characters the mnemonic is, which is (99.9% of the time)
		 * the number of bytes it is.
		 * 
		 * We do not store null terminators, so this number is the exact number of
		 * characters stored.
		 */
		const SABOT_HUINTN mnemonicLength;
		
		/**
		 * \brief The number of operands the instruction requires.
		 */
		const SABOT_HUINTN operandCount;
		
		/**
		 * \brief A bytecode representation of the instruction.
		 * 
		 * Of course this is guarenteed to be unique, and the type is specified elsewhere
		 * in the description of a machine.
		 */
		const machine::bytecode_t bytecode;
	};
}

#endif
