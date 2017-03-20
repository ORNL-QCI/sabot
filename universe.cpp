#include "universe.hpp"

universe& universe::here() {
    static universe instance;
    return instance;
}

universe::universe() {
}

universe::~universe() {
}

void universe::initialize_interpreter(const char *dialect) {
    if(interpreters.count(dialect) == 0) {
        interpreters.insert(dialect, new language::interpreter(dialect));
    }
}

language::program universe::compile_program(const char *dialect, 
                                            const char *data,
                                            char lineDelimiter) {
    initialize_interpreter(dialect);
    
    return interpreters.edit_item(dialect,
            [&](language::interpreter &item)->language::program {
                return item.parse_program(data, lineDelimiter);
            });
}

std::uint64_t universe::compile_macro(kernel::id_t kernelId,
                                      const char *dialect,
                                      const char *data,
                                      char lineDelimiter) {
    initialize_interpreter(dialect);
    
    return kernels.edit_item(kernelId,
        [&](kernel &item)->std::uint64_t {
            auto prgrm = compile_program(dialect, data, lineDelimiter);
            return item.insert_macro(std::move(prgrm));
        });
}

kernel::id_t universe::create_kernel() {
    return kernels.insert(
            [&]()->kernel* {
                return new kernel();
            });
}

bool universe::delete_kernel(kernel::id_t kernelId) {
    return kernels.erase(kernelId);
}

quantum_system::id_t universe::create_system(const char *stateType) {
    return quantumSystems.insert(
            [stateType]()->quantum_system* {
                return new quantum_system(stateType);
            });
}

bool universe::delete_system(const quantum_system::id_t systemId) {
    return quantumSystems.erase(systemId);
}
	
std::uint_fast64_t universe::create_state(quantum_system::id_t systemId,
                                          const char *dialect,
                                          const char *data,
                                          const char lineDelimiter) {
    try {
        // Return the state id
        return quantumSystems.edit_item(systemId,
                [&](quantum_system &item)->std::uint_fast64_t {
                    auto prgrm = compile_program(dialect, data, lineDelimiter);
                    return item.state_type().program_insert_state(std::move(prgrm));
                });
    } catch(const std::out_of_range& e) {
        // System was not found
        throw std::exception();
    }
}

bool universe::modify_state(quantum_system::id_t systemId,
                            std::uint_fast64_t stateId,
                            const char *dialect, 
                            const char *data,
                            char lineDelimiter) {
    try {
        return quantumSystems.edit_item(systemId,
                [&](quantum_system &item)->bool {
                    auto prgrm = compile_program(dialect, data, lineDelimiter);
                    return item.state_type().program_modify_state(stateId, std::move(prgrm));
                });
    } catch(const std::out_of_range& e) {
        // System was not found
        return false;
    }
}

bool universe::measure_state(quantum_system::id_t systemId,
                             std::uint_fast64_t stateId,
                             const char *dialect,
                             const char *data,
                             char lineDelimiter,
                             string_buffer<>& buffer) {
    try {
        return quantumSystems.edit_item(systemId,
                [&](quantum_system &item)->bool {
                    auto prgrm = compile_program(dialect, data, lineDelimiter);
                    return item.state_type().program_measure_state(stateId, std::move(prgrm), buffer);
                });
    } catch(const std::out_of_range& e) {
        // System was not found
        return false;
    }
}

bool universe::delete_state(quantum_system::id_t systemId,
                            std::uint_fast64_t stateId) {
    try {
       return quantumSystems.edit_item(systemId,
               [&](quantum_system &item)->bool {
                   return item.state_type().delete_state_element(stateId);
               });
    } catch(const std::out_of_range& e) {
        // System was not found
        return false;
    }
}

bool universe::compute_result(quantum_system::id_t systemId,
                              const char *dialect, 
                              const char *data,
                              char lineDelimiter,
                              string_buffer<>& buffer) {
    try {
        return quantumSystems.edit_item(systemId,
                [&](quantum_system &item)->bool {
                    auto prgrm = compile_program(dialect, data, lineDelimiter);
                    return item.state_type().program_compute_result(std::move(prgrm), buffer);
                });
    } catch(const std::out_of_range& e) {
        // System was not found
        return false;
    }
}
