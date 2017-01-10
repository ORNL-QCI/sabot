#ifndef _COMMON_H
#define _COMMON_H

#include <cassert>
#include <cstring>

#if __x86_64__ || \
	_M_X64 || \
	_WIN64 || \
	__powerpc64__ || \
	__ppc64__ || \
	__PPC64__ || \
	__ARCH_PPC64 || \
	__aarch64__ || \
	__sparc64__
#	define SABOT_64BIT
#else
#	define SABOT_32BIT
#endif

#if defined __GNUC__ || defined __clang__
#	define LIKELY(x)	__builtin_expect(x, 1)
#	define UNLIKELY(x)	__builtin_expect(x, 0)
#else
#	define LIKELY(x)	(x)
#	define UNLIKELY(x)	(x)
#endif

#define UNUSED(x) (void)(x)

/**
 * \brief Loop unroll hint for the compiler.
 * 
 * This has to be put before a function declaration.
 */
#if defined __GNUC__
#	define UNROLL_LOOP __attribute__((optimize("unroll-loops")))
#else
#	define UNROLL_LOOP
#endif

#if defined(__SSE4_2__)
/**
 * \brief Enable SSE4.2 optimization for rapidjson.
 */
#	define RAPIDJSON_SSE42
#elif defined(__SSE2__)
/**
 * \brief Enable SSE2 optimization for rapidjson.
 */
#	define RAPIDJSON_SSE2
#endif

/**
 * \brief Disable assert in rapidjson.
 */
#define RAPIDJSON_ASSERT(x) (void)(x)

#ifdef SABOT_64BIT
#	define SABOT_UINTN uint64_t
#	define SABOT_HUINTN uint32_t
#	define SABOT_QUINTN uint16_t
#else
#	define SABOT_UINTN uint32_t
#	define SABOT_HUINTN uint16_t
#	define SABOT_QUINTN uint8_t
#endif

/**
 * \brief Inform the RNG we are not using threads.
 */
//#define SABOT_NTHRD_RNG 1

#endif