#ifndef _LANGUAGE_PROGRAM_HPP
#define _LANGUAGE_PROGRAM_HPP

#include <common.hpp>
#include <managed_container.hpp>
#include "instruction.hpp"

namespace language {
	/**
	 * \brief Program consisting of instructions.
	 */
	struct program {
	 public:
		using id_t = std::uint_fast64_t;
		
		/**
		 * \brief Constructor that takes a name and a list of instructions.
		 * It accepts an array of instructions via move semantics.
		 */
		program(language::instruction*&& instruction,
				const std::size_t size) {
			_instructions.add(instruction, size);
			delete[] instruction;
		}
		
		/**
		 * \brief Copy constructor is disabled.
		 */
		program(const program&) = delete;
		
		 /**
		 * \brief Move constructor.
		 */
		program(program&& old)
				: _instructions(std::move(old._instructions)) {
		}
		
		 /**
		 * \brief Assignment operator is disabled.
		 */
		program& operator=(const program&) = delete; 
		
		 /**
		 * \brief Move assignment operator.
		 */
		program& operator=(program&& old) {
			_instructions = std::move(old._instructions);
			
			return *this;
		}
		
		/**
		 * \brief Destructor.
		 */
		~program() {
		}
		
		/**
		 * \brief Return the number of instructions.
		 */
		inline std::size_t size() const {
			return _instructions.count();
		}
		
		/**
		 * \brief Return an instruction at a given address.
		 */
		inline const language::instruction& instruction(const SABOT_UINTN addr) const {
			#ifdef SABOT_THROW
			if(UNLIKELY(addr >= size())) {
				throw std::invalid_argument(err_msg::_arybnds);
			}
			#endif
			
			return _instructions.data()[addr];
		}
	
	 private:
		managed_container<language::instruction, SABOT_UINTN, 16> _instructions;
	};
}

#endif
