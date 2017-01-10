#ifndef _MACHINE_HPP
#define _MACHINE_HPP

#include <common.hpp>

/**
 * \brief Helper m acro to define the number of operands required for a given operation.
 * 
 * It cleans up clutter from the storage class keywords, ensures we have an entry within
 * the operation enum that corresponds to the variable name defined here, and ensures our
 * defined number of operands does not exceed our maximum.
 */
#define MACHINE_OPERANDC_DEF(name, count) \
		static_assert(operation::name==operation::name, "operation not in enum"); \
		static_assert(max_operand_count>=count, "maximum operand count exceeded"); \
		constexpr static const SABOT_UINTN name = count

/**
 * \brief Helper macro to return the number of operands required for a given operation.
 */
#define MACHINE_OPERANDC_HELPER(op) ::machine::operand_counts::op

/**
 * \brief Helper macro to return the bytecode of a given operation.
 */
#define MACHINE_BYTECODE_HELPER(op) ::machine::operation::op

namespace machine {
	/**
	 * \brief This enum serves a dual purpose: to provide a comprehensive list of
	 * operations a machine may perform and provide a unique bytecode to each of those
	 * operations.
	 * 
	 * Using an enum ensures all bytecode values are unique and generated at compile-time,
	 * therefore, no items within this enumeration should have a value explicitly defined
	 * and no code referencing this enumeration should rely on explicit values.
	 * 
	 * \warning The space of mnemonics is far larger than the size of bytecodes and under
	 * sane circumstances this should never be encountered, but it is possible.
	 */
	enum class operation {
		macro,
		initialize,
		identity,
		hadamard,
		phase,
		measure,
		cnot,
		paulix,
		pauliy,
		pauliz,
	};
	
	/**
	 * \brief Type alias for bytecode storage.
	 */
	using bytecode_t = operation;
	
	/**
	 * \brief The type an instruction operand is stored as.
	 * 
	 * This doesn't say anything about how that operand is interpreted, just what the
	 * storage type.
	 */
	using operand_t = SABOT_UINTN;
	
	/**
	 * \brief The maximum number of operands an instruction may have.
	 * 
	 * An instruction doesn't have to use this many operands, but it cannot use more
	 * than it.
	 */
	constexpr static const SABOT_UINTN max_operand_count = 2;
	
	/**
	 * \brief Define the number of operands an operation requires.
	 *
	 * \note Only use the macro to define members!
	 */
	struct operand_counts {
	 public:
		MACHINE_OPERANDC_DEF(macro, 2);
		MACHINE_OPERANDC_DEF(initialize, 1);
		MACHINE_OPERANDC_DEF(identity, 1);
		MACHINE_OPERANDC_DEF(hadamard, 1);
		MACHINE_OPERANDC_DEF(phase, 1);
		MACHINE_OPERANDC_DEF(measure, 1);
		MACHINE_OPERANDC_DEF(cnot, 2);
		MACHINE_OPERANDC_DEF(paulix, 1);
		MACHINE_OPERANDC_DEF(pauliy, 1);
		MACHINE_OPERANDC_DEF(pauliz, 1);
	};
}

#endif