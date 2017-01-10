#include "kernel.hpp"

kernel::kernel() {
}

kernel::kernel(kernel&& old) {
	UNUSED(old);
}

kernel& kernel::operator=(kernel&& old) {
	UNUSED(old);
	
	return *this;
}

kernel::~kernel() {
}

std::uint64_t kernel::insert_macro(language::program&& macro) {
	return _macros.insert(std::move(macro));
}
	
const language::program& kernel::get_macro(std::uint64_t macroId) {
	std::mutex* kernelElementMutex = 0;
	boost::shared_mutex* kernelListMutex = 0;

	return _macros.get(macroId, kernelElementMutex, kernelListMutex);
}