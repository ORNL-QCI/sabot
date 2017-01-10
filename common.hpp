#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <common.h>
#include <stdexcept>


//#define SABOT_THROW

/**
 * \brief Error message strings.
 */
namespace err_msg {
	const char _nllpntr[] = "null ptr";
	
	const char _badvals[] = "bad values";
	const char _unrchcd[] = "unreachable code reached";
	
	const char _arybnds[] = "array bounds exceeded";
	const char _pmtrcnt[] = "bad parameter count";
	const char _objimpl[] = "object unimplemented in child";
	const char _tpntfnd[] = "type not found by name";
	const char _rgstrfl[] = "failed to register child";
	const char _undhcse[] = "unhandled case";
	const char _zrlngth[] = "zero length";
	const char _ntwrkdn[] = "network down";
	const char _mnltrmr[] = "missing null terminator";
}


#endif
