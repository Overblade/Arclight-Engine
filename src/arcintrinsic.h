#pragma once

#include "arcbuild.h"

#if defined(ARC_PLATFORM_X86) && defined(ARC_COMPILER_MSVC)
	/*
		For _mm_pause
	*/
	#include <immintrin.h>
#endif

#ifdef ARC_COMPILER_MSVC
	/*
		For __nop
	*/
	#include <intrin.h>
#endif


/*
	Defines ARC_FORCE_INLINE
	Forces the compiler to inline a certain function.
*/
#ifdef ARC_COMPILER_MSVC
	#define ARC_FORCE_INLINE	__forceinline
#elif defined(ARC_COMPILER_CLANG) || defined(ARC_COMPILER_GCC)
	#define ARC_FORCE_INLINE	__attribute__((always_inline)) inline
#else
	#define ARC_FORCE_INLINE	inline
#endif


/*
	Defines ARC_INTRINSIC_NOP
	Emits a nop instruction.
*/
#ifdef ARC_COMPILER_MSVC
	#define ARC_INTRINSIC_NOP()	__nop()
#elif defined(ARC_COMPILER_CLANG) || defined(ARC_COMPILER_GCC)
	#define ARC_INTRINSIC_NOP()	asm volatile("nop" ::: "memory")
#else
	#define ARC_INTRINSIC_NOP()
#endif


/*
	arc_spin_yield()
	Inserts a yielding instruction to optimize spinlocks.
*/
ARC_FORCE_INLINE void arc_spin_yield() noexcept {

#ifdef ARC_PLATFORM_X86
	_mm_pause();
#elif ARC_PLATFORM_ARM
	__yield();
#else
	ARC_INTRINSIC_NOP();
#endif

}