#ifndef _LIBFIXMATH_CONF_H
#define _LIBFIXMATH_CONF_H

#if defined(__arm__) 

//#define FIXMATH_NO_64BIT
//#define FIXMATH_OPTIMIZE_8BIT
//#define FIXMATH_NO_CACHE
#define FIXMATH_NO_OVERFLOW
//#define FIXMATH_NO_ROUNDING

#elif defined(__AVR__) 

// AVR Architecture is 8bit & limited in memory
#define FIXMATH_NO_64BIT
#define FIXMATH_OPTIMIZE_8BIT
#define FIXMATH_NO_CACHE
#define FIXMATH_NO_OVERFLOW
//#define FIXMATH_NO_ROUNDING

#else
#error Architecture or board not supported.
#endif

#endif
