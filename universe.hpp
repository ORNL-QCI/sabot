#ifndef _UNIVERSE_HPP
#define _UNIVERSE_HPP

#include <common.hpp>
#include "kernel.hpp"
#include "quantum_system.hpp"
#include "randnum/generator.hpp"
#include "language/interpreter.hpp"
#include "ms_container.hpp"
#include "string_buffer.hpp"

/**
 * \brief This class acts as a daemon in the code through being singleton,
 * providing the interface to the quantum simulation code.
 */
class universe {
 public:
	/**
	 * \brief Singleton instance function.
	 */
	static universe& here();
	
	/**
	 * \brief Destructor.
	 */
	~universe();
	
	/**
	 * \brief Return an instance to the random number generator.
	 * 
	 * \returns The randnum generator.
	 */
	inline randnum::generator& random_generator() {
		return generator;
	}
	
	/**
	 * \brief Return an interpreter for a particular dialect.
	 * 
	 * If we currently do not have one stored, we create it.
	 * 
	 * \returns The langauge interpreter for the specified dialect.
	 */
	const language::interpreter& interpreter(const char* const dialect,
			std::mutex*& elementMutex, boost::shared_mutex*& listMutex);
	
	/**
	 * \brief Return a kernel by its id.
	 *
	 * \returns The kernel.
	 */
	kernel& get_kernel(kernel::id_t kernelId,
		std::mutex*& elementMutex,
		boost::shared_mutex*& listMutex);
	
	/**
	 * \brief Compile a program and return it.
	 * 
	 * \returns The compiled program.
	 */
	language::program compile_program(const char* const dialect, 
			const char* const data,
			const char lineDelimiter);
	
	/**
	 * \brief Compile a macro, insert it into the kernel, and return the id of it.
	 *
	 * \returns The id of the macro.
	 */
	std::uint64_t compile_macro(const kernel::id_t kernelId,
		const char* const dialect,
		const char* const data,
		const char lineDelimiter);
	
	const language::program& get_macro(const kernel::id_t kernelId,
			const std::uint64_t macroId);
	
	/**
	 * \brief \todo
	 */
	kernel::id_t create_kernel();
	
	/**
	 * \brief \todo
	 */
	bool delete_kernel(const kernel::id_t kernelId);
	
	/**
	 * \brief Create a system with a specified state_type and noise_type by name.
	 */
	quantum_system::id_t create_system(const char* stateType);
	
	/**
	 * \brief Delete a quantum system from its systemId.
	 */
	bool delete_system(const quantum_system::id_t systemId);
	
	/**
	 * \brief Create a state within a quantum system.
	 * 
	 * \returns The Id of the new state.
	 * 
	 * \throws std::exception Signal we could not insert the new state into our
	 * 	       collection.
	 */
	std::uint_fast64_t create_state(const quantum_system::id_t systemId,
			const char* const dialect,
			const char* const data,
			const char lineDelimiter);
	
	/**
	 * \brief Modify an existing state.
	 * 
	 * \returns Whether or not the state could be modified. If the system or
	 * state was not found, return false.
	 */
	bool modify_state(const quantum_system::id_t systemId,
			const std::uint_fast64_t stateId,
			const char* const dialect, 
			const char* const data,
			const char lineDelimiter);
	
	/**
	 * \brief Measure a state.
	 * 
	 * Results of the measurement will be stored in the buffer argument supplied.
	 * 
	 * \returns Whether or not the state could be measured. If the system or
	 * state was not found, return false.
	 */
	bool measure_state(const quantum_system::id_t systemId,
			const std::uint_fast64_t stateId,
			const char* const dialect, 
			const char* const data,
			const char lineDelimiter,
			string_buffer<>& buffer);
	
	/**
	 * \brief Delete a state within a quantum system specified from its id.
	 * 
	 * \returns Whether or not the state could be deleted. If the system or
	 * state was not found, return false.
	 */
	bool delete_state(const quantum_system::id_t systemId,
			const std::uint_fast64_t stateId);
	
	/**
	 * \brief Compute the result of a program without storing a state.
	 * 
	 * \returns Whether or not the operation was successful. If the system was
	 * not found, returns false. Results of the measurement will be stored in
	 * the buffer argument supplied.
	 */
	bool compute_result(const quantum_system::id_t systemId,
			const char* const dialect, 
			const char* const data,
			const char lineDelimiter,
			string_buffer<>& buffer);

 private:
	/**
	 * \brief Constructor.
	 */
	universe();
	
	/**
	 * \brief Copy constructor is disabled.
	 */
	universe(const universe&) = delete;
	
	/**
	 * \brief Move constructor is disabled.
	 */
	universe(universe&&) = delete;
	
	/**
	 * \brief Copy assignment operator is disabled.
	 */
	universe& operator=(const universe&) = delete;
	
	/**
	 * \brief Move assignment operator is disabled.
	 */
	universe& operator=(universe&&) = delete;
	
	/**
	 * \brief By keeping our RNG here, we ensure only one instance of an RNG
	 * is active throughout execution.
	 * 
	 * \note Threadsafe
	 */
	randnum::generator generator;
	
	/**
	 * \brief The storage of quantum systems and by extension any states within them.
	 * 
	 * \note Threadsafe
	 */
	ms_container<quantum_system::id_t, quantum_system, 8> quantumSystems;
	
	/**
	 * \brief A collection of interpreters with a given dialect. We populate this
	 * collection as needed, and check it each time we look for an interpreter.
	 * There is no need to create a new interpreter each time.
	 * 
	 * \note Threadsafe
	 */
	ms_container<language::interpreter::id_t, language::interpreter, 2, true> interpreters;
	
	/**
	 * \brief A collection of processing kernels.
	 *
	 * \note Threadsafe
	 */
	ms_container<kernel::id_t, kernel, 8> kernels;
};

#endif
