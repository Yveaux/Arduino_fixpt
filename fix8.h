#ifndef __libfixmath_fix8_h__
#define __libfixmath_fix8_h__

#ifdef __cplusplus
extern "C"
{
#endif

/* These options may let the optimizer to remove some calls to the functions.
 * Refer to http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
 */
#ifndef FIXMATH_FUNC_ATTRS
# ifdef __GNUC__
#   if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 6)
#     define FIXMATH_FUNC_ATTRS __attribute__((leaf, nothrow, const))
#   else
#     define FIXMATH_FUNC_ATTRS __attribute__((nothrow, const))
#   endif
# else
#   define FIXMATH_FUNC_ATTRS
# endif
#endif

#include "libfixmath_conf.h"
#include <stdint.h>

typedef int16_t fix8_t;
#if 0
static const fix8_t FOUR_DIV_PI  = 0x145F3;            /*!< Fix8 value of 4/PI */
static const fix8_t _FOUR_DIV_PI2 = 0xFFFF9840;        /*!< Fix8 value of -4/PI² */
static const fix8_t X4_CORRECTION_COMPONENT = 0x399A; 	/*!< Fix8 value of 0.225 */
static const fix8_t PI_DIV_4 = 0x0000C90F;             /*!< Fix8 value of PI/4 */
static const fix8_t THREE_PI_DIV_4 = 0x00025B2F;       /*!< Fix8 value of 3PI/4 */
#endif

static const fix8_t fix8_maximum  = 0x7FFF; /*!< the maximum value of fix8_t */
static const fix8_t fix8_minimum  = 0x8000; /*!< the minimum value of fix8_t */
static const fix8_t fix8_overflow = 0x8000; /*!< the value used to indicate overflows when FIXMATH_NO_OVERFLOW is not specified */

#if 0
static const fix8_t fix8_pi  = 205887;     /*!< fix8_t value of pi */
static const fix8_t fix8_e   = 178145;     /*!< fix8_t value of e */
#endif
static const fix8_t fix8_one = 0x0100;     /*!< fix8_t value of 1 */

/* Conversion functions between fix8_t and float/integer.
 * These are inlined to allow compiler to optimize away constant numbers
 */
static inline fix8_t  fix8_from_int(int8_t a) { return a * fix8_one; }
static inline float   fix8_to_float(fix8_t a) { return (float)a / fix8_one; }
static inline double  fix8_to_dbl(fix8_t a)   { return (double)a / fix8_one; }

static inline int8_t fix8_to_int(fix8_t a)
{
#ifdef FIXMATH_NO_ROUNDING
    return (a >> 8);
#else
	if (a >= 0)
		return (a + (fix8_one >> 1)) / fix8_one;
	return (a - (fix8_one >> 1)) / fix8_one;
#endif
}

static inline fix8_t fix8_from_float(float a)
{
	float temp = a * fix8_one;
#ifndef FIXMATH_NO_ROUNDING
	temp += (temp >= 0) ? 0.5f : -0.5f;
#endif
	return (fix8_t)temp;
}

static inline fix8_t fix8_from_dbl(double a)
{
	double temp = a * fix8_one;
#ifndef FIXMATH_NO_ROUNDING
	temp += (temp >= 0) ? 0.5f : -0.5f;
#endif
	return (fix8_t)temp;
}

/* Macro for defining fix8_t constant values.
   The functions above can't be used from e.g. global variable initializers,
   and their names are quite long also. This macro is useful for constants
   springled alongside code, e.g. F8(1.234).

   Note that the argument is evaluated multiple times, and also otherwise
   you should only use this for constant values. For runtime-conversions,
   use the functions above.
*/
#define F8(x) ((fix8_t)(((x) >= 0) ? ((x) * 256.0 + 0.5) : ((x) * 256.0 - 0.5)))

static inline fix8_t fix8_abs(fix8_t x)
	{ return (x < 0 ? -x : x); }
static inline fix8_t fix8_floor(fix8_t x)
	{ return (x & 0xFF00UL); }
static inline fix8_t fix8_ceil(fix8_t x)
	{ return (x & 0xFF00UL) + (x & 0x00FFUL ? fix8_one : 0); }
static inline fix8_t fix8_min(fix8_t x, fix8_t y)
	{ return (x < y ? x : y); }
static inline fix8_t fix8_max(fix8_t x, fix8_t y)
	{ return (x > y ? x : y); }
static inline fix8_t fix8_clamp(fix8_t x, fix8_t lo, fix8_t hi)
	{ return fix8_min(fix8_max(x, lo), hi); }

/* Subtraction and addition with (optional) overflow detection. */
#ifdef FIXMATH_NO_OVERFLOW

static inline fix8_t fix8_add(fix8_t inArg0, fix8_t inArg1) { return (inArg0 + inArg1); }
static inline fix8_t fix8_sub(fix8_t inArg0, fix8_t inArg1) { return (inArg0 - inArg1); }

#else

extern fix8_t fix8_add(fix8_t a, fix8_t b) FIXMATH_FUNC_ATTRS;
extern fix8_t fix8_sub(fix8_t a, fix8_t b) FIXMATH_FUNC_ATTRS;

/* Saturating arithmetic */
extern fix8_t fix8_sadd(fix8_t a, fix8_t b) FIXMATH_FUNC_ATTRS;
extern fix8_t fix8_ssub(fix8_t a, fix8_t b) FIXMATH_FUNC_ATTRS;

#endif

/*! Multiplies the two given fix8_t's and returns the result.
*/
extern fix8_t fix8_mul(fix8_t inArg0, fix8_t inArg1) FIXMATH_FUNC_ATTRS;

/*! Divides the first given fix8_t by the second and returns the result.
*/
extern fix8_t fix8_div(fix8_t inArg0, fix8_t inArg1) FIXMATH_FUNC_ATTRS;

#ifndef FIXMATH_NO_OVERFLOW
/*! Performs a saturated multiplication (overflow-protected) of the two given fix8_t's and returns the result.
*/
extern fix8_t fix8_smul(fix8_t inArg0, fix8_t inArg1) FIXMATH_FUNC_ATTRS;

/*! Performs a saturated division (overflow-protected) of the first fix8_t by the second and returns the result.
*/
extern fix8_t fix8_sdiv(fix8_t inArg0, fix8_t inArg1) FIXMATH_FUNC_ATTRS;
#endif

/*! Divides the first given fix8_t by the second and returns the result.
*/
extern fix8_t fix8_mod(fix8_t x, fix8_t y) FIXMATH_FUNC_ATTRS;



/*! Returns the linear interpolation: (inArg0 * (1 - inFract)) + (inArg1 * inFract)
*/
extern fix8_t fix8_lerp8(fix8_t inArg0, fix8_t inArg1, uint8_t inFract) FIXMATH_FUNC_ATTRS;
extern fix8_t fix8_lerp16(fix8_t inArg0, fix8_t inArg1, uint16_t inFract) FIXMATH_FUNC_ATTRS;
#ifndef FIXMATH_NO_64BIT
extern fix8_t fix8_lerp32(fix8_t inArg0, fix8_t inArg1, uint32_t inFract) FIXMATH_FUNC_ATTRS;
#endif



/*! Returns the sine of the given fix8_t.
*/
extern fix8_t fix8_sin_parabola(fix8_t inAngle) FIXMATH_FUNC_ATTRS;

/*! Returns the sine of the given fix8_t.
*/
extern fix8_t fix8_sin(fix8_t inAngle) FIXMATH_FUNC_ATTRS;

/*! Returns the cosine of the given fix8_t.
*/
extern fix8_t fix8_cos(fix8_t inAngle) FIXMATH_FUNC_ATTRS;

/*! Returns the tangent of the given fix8_t.
*/
extern fix8_t fix8_tan(fix8_t inAngle) FIXMATH_FUNC_ATTRS;

/*! Returns the arcsine of the given fix8_t.
*/
extern fix8_t fix8_asin(fix8_t inValue) FIXMATH_FUNC_ATTRS;

/*! Returns the arccosine of the given fix8_t.
*/
extern fix8_t fix8_acos(fix8_t inValue) FIXMATH_FUNC_ATTRS;

/*! Returns the arctangent of the given fix8_t.
*/
extern fix8_t fix8_atan(fix8_t inValue) FIXMATH_FUNC_ATTRS;

/*! Returns the arctangent of inY/inX.
*/
extern fix8_t fix8_atan2(fix8_t inY, fix8_t inX) FIXMATH_FUNC_ATTRS;

static const fix8_t fix8_rad_to_deg_mult = 14668; /* 3754936 / 256 */
static inline fix8_t fix8_rad_to_deg(fix8_t radians)
	{ return fix8_mul(radians, fix8_rad_to_deg_mult); }

static const fix8_t fix8_deg_to_rad_mult = 4;   /* 1144 / 256 */
static inline fix8_t fix8_deg_to_rad(fix8_t degrees)
	{ return fix8_mul(degrees, fix8_deg_to_rad_mult); }



/*! Returns the square root of the given fix8_t.
*/
extern fix8_t fix8_sqrt(fix8_t inValue) FIXMATH_FUNC_ATTRS;

/*! Returns the square of the given fix8_t.
*/
static inline fix8_t fix8_sq(fix8_t x)
	{ return fix8_mul(x, x); }

/*! Returns the exponent (e^) of the given fix8_t.
*/
extern fix8_t fix8_exp(fix8_t inValue) FIXMATH_FUNC_ATTRS;

/*! Returns the natural logarithm of the given fix8_t.
 */
extern fix8_t fix8_log(fix8_t inValue) FIXMATH_FUNC_ATTRS;

/*! Returns the base 2 logarithm of the given fix8_t.
 */
extern fix8_t fix8_log2(fix8_t x) FIXMATH_FUNC_ATTRS;

/*! Returns the saturated base 2 logarithm of the given fix8_t.
 */
extern fix8_t fix8_slog2(fix8_t x) FIXMATH_FUNC_ATTRS;

/*! Convert fix8_t value to a string.
 * Required buffer length for largest values is 13 bytes.
 */
extern void fix8_to_str(fix8_t value, char *buf, int decimals);

/*! Convert string to a fix8_t value
 * Ignores spaces at beginning and end. Returns fix8_overflow if
 * value is too large or there were garbage characters.
 */
extern fix8_t fix8_from_str(const char *buf);

#ifdef __cplusplus
}
#include "fix8.hpp"
#endif

#endif
