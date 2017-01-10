#ifndef _LANGUAGE_DIALECT_DIALECT_HPP
#define _LANGUAGE_DIALECT_DIALECT_HPP

#include <common.hpp>
#include "instruction_definition.hpp"
#include <map>

namespace language {
	/**
	 * \brief Base language dialect for the interpreter.
	 */
	class dialect {
	 public:
		/**
		 * \brief A dialect is constructed with two delimiters, the first
		 * for use between the opcode and operand, and the second for use
		 * between operands.
		 */
		dialect(const char opcodeDelimiter,
				const char operandDelimiter)
				: _opcodeDelimiter(opcodeDelimiter),
				_operandDelimiter(operandDelimiter),
				_instructionDefCount(0),
				_instructionDefs(0) {
		}
		
		/**
		 * \brief Get the delimiter that is used between the opcode
		 * and the operand.
		 */
		inline char opcode_delimiter() const {
			return _opcodeDelimiter;
		}
			
		/**
		 * \brief Get the delimiter that is used between operands.
		 */
		inline char operand_delimiter() const {
			return _operandDelimiter;
		}
		
		inline const instruction_definition& find_idef(const char* const str, const std::size_t len) const {
			for(std::size_t i = 0; i < _instructionDefCount; i++) {
				if(_instructionDefs[i].mnemonicLength == len &&
				
				memcmp(_instructionDefs[i].mnemonic, str, len) == 0) {
					return _instructionDefs[i];
				}
			}
			
			throw std::exception();
		}
	
	 protected:
		inline void register_instructions(const instruction_definition list[], const std::size_t count) {
			_instructionDefs = list;
			_instructionDefCount = count;
		}
	
	 private:
		char _opcodeDelimiter;
		char _operandDelimiter;
		std::size_t _instructionDefCount;
		const instruction_definition* _instructionDefs;
		
	};
	
	/**
	 * \brief Create a new dialect from the template parameter.
	 */
	template<typename T> dialect* create_dialect() {
		return new T;
	}
	
	/**
	 * \brief Factory to create and keep track of dialects.
	 */
	struct dialect_factory {
	 public:
		using dialect_map_type = std::map<const char*, dialect*(*)()>;
		
		/**
		 * \brief Instantiate a dialect from its name.
		 */
		static dialect* instantiate(const char* const name) {
			for(auto it : dialect_map()) {
				if(strcmp(it.first, name) == 0) {
					return it.second();
				}
			}
			throw std::exception();
		}
		
	 protected:
		/**
		 * \brief Return the map of dialects.
		 */
		static dialect_map_type& dialect_map() {
			return _dialect_map;
		}
	
	 private:
		static dialect_map_type _dialect_map;
	};
	
	/**
	 * \brief Template to register a dialect in the dialect factory.
	 * The template parameter is the dialect type.
	 */
	template<typename T> struct dialect_register : dialect_factory {
	 public:
		/**
		 * \brief Constructor takes a name of the dialect to be
		 * referenced when instantiating later.
		 */
		dialect_register(const char* const name) {
			auto it(dialect_map().insert(std::make_pair(name, &create_dialect<T>)));
			assert(it.second == true);
		}
	};
}

#endif
