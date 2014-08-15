#ifndef _LIBFIXMATH_CONF_H
#define _LIBFIXMATH_CONF_H

#ifdef ARDUINO_ARCH_AVR
// ARDUINO_ARCH_AVR might not be the optimum define to check on,
// but nothing else is available...
// AVR Architecture is 8bit & limited in memory
#define FIXMATH_NO_64BIT
#define FIXMATH_OPTIMIZE_8BIT
#define FIXMATH_NO_CACHE
//#define FIXMATH_NO_OVERFLOW
//#define FIXMATH_NO_ROUNDING
#endif

#endif
