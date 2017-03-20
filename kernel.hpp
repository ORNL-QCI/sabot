#ifndef _KERNEL_HPP
#define _KERNEL_HPP

#include <common.hpp>
#include "ms_container.hpp"
#include "language/program.hpp"

class quantum_system;

class kernel {
 public:
	using id_t = std::size_t;
	
	/**
	 * \brief Constructor.
	 */
	kernel();
	
	/**
	 * \brief Copy constructor is disabled.
	 */
	kernel(const kernel&) = delete;
	
	/**
	 * \brief Move constructor is disabled.
	 */
	kernel(kernel&& old);
	
	/**
	 * \brief Copy assignment operator is disabled.
	 */
	kernel& operator=(const kernel&) = delete;
	
	/**
	 * \brief Move assignment operator is disabled.
	 */
	kernel& operator=(kernel&& old);
	
	/**
	 * \brief Destructor.
	 */
	~kernel();
	
	/**
	 * \brief \todo
	 */
	std::uint64_t insert_macro(language::program&& macro);
	
 private:
	/**
	 * \brief \todo
	 */
	id_t _id;
	
	/**
	 * \brief A collection of compile programs for the kernel.
	 * 
	 * \note Threadsafe
	 */
	ms_container<language::program::id_t, language::program> _macros;
};

#endif
