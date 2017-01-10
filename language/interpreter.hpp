#ifndef _LANGUAGE_INTERPRETER_HPP
#define _LANGUAGE_INTERPRETER_HPP

#include <common.hpp>
#include <utility.hpp>
#include "dialect.hpp"
#include "dialects/dialect_list.hpp"
#include "instruction.hpp"
#include "program.hpp"

namespace language {
	/**
	 * \brief The interpreter converts assembly into an instruction based
	 * upon some rules of the chosen dialect.
	 */
	class interpreter {
	 public:
		using id_t = std::uint_fast64_t;
		
		/**
		 * \brief Constructor takes the string name of the dialect and
		 * instantiates an instance from the dialect factory.
		 */
		interpreter(const char* const dialectName) {
			_dialect = dialect_factory::instantiate(dialectName); 
		}
		
		/**
		 * \brief Copy constructor is disabled.
		 */
		interpreter(const interpreter&) = delete;
		
		/**
		 * \brief Move constructor.
		 */
		interpreter(interpreter&& old) {
			_dialect = old._dialect;
			old._dialect = 0;
		}
		
		/**
		 * \brief Copy assignment operator is disabled.
		 */
		interpreter& operator=(const interpreter&) = delete;
		
		/**
		 * \brief Move assignment operator.
		 */
		interpreter& operator=(interpreter&& old) {
			_dialect = old._dialect;
			old._dialect = 0;
			return *this;
		}
		
		/**
		 * \brief Destructor destroys our dialect object.
		 */
		~interpreter() {
			if(_dialect != 0) {
				delete _dialect;
			}
		}
		
		program parse_program(const char* const lines, const char lineDelimiter) const;
		
		inline const ::language::dialect& dialect() const {
			return *_dialect;
		}
	
	 private:
		::language::dialect* _dialect;
		
		/**
		 * \brief Move a pointer up until a character is not a space or a tab.
		 */
		static const char* remove_leading_whitespace(const char* buffer) {
			while(*buffer != '\0' && (*buffer == ' ' || *buffer == '\t')) {
				buffer++;
			}
			return buffer;
		}
	};
}

#endif
