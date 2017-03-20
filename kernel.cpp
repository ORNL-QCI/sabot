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
	/** \TODO fixme */
	return _macros.insert(new language::program(std::move(macro)));
}
