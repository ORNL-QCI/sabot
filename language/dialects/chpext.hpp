#ifndef _LANGUAGE_DIALECTS_CHPEXT_HPP
#define _LANGUAGE_DIALECTS_CHPEXT_HPP

#include <common.hpp>
#include "../dialect.hpp"

namespace language {
	namespace dialects {
		/**
		 * \brief The language first used with Scott Aarsonson's CHP  program. It is a
		 * subset of the QASM language.
		 */
		class chpext : public ::language::dialect {
		 public:
			/**
			 * \brief Constructor registers the properties of the dialect.
			 */
			chpext();
		
		 private:
			/**
			 * \brief Register dialect with the dialect factory.
			 */
			static const dialect_register<chpext> name;
			
			/**
			 * \brief Define the character delimiter between an opcode and operands.
			 */
			static const char opcodeDelimiter;
			
			/**
			 * \brief Define the character delimiter between operands.
			 */
			static const char operandDelimiter;
			
			/**
			 * \brief List of mappings between mnemonics and operators.
			 */
			INSDEF_LISTD_HELPER(instructionDefs);
		};
	}
}

#endif
