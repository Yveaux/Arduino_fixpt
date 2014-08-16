#include "fix8.h"
#if !defined(FIXMATH_NO_64BIT)
#include "int64.h"
#endif


/* Subtraction and addition with overflow detection.
 * The versions without overflow detection are inlined in the header.
 */
#ifndef FIXMATH_NO_OVERFLOW
fix8_t fix8_add(fix8_t a, fix8_t b)
{
	// Use unsigned integers because overflow with signed integers is
	// an undefined operation (http://www.airs.com/blog/archives/120).
	uint16_t _a = a, _b = b;
	uint16_t sum = _a + _b;

	// Overflow can only happen if sign of a == sign of b, and then
	// it causes sign of sum != sign of a.
	if (!((_a ^ _b) & 0x8000) && ((_a ^ sum) & 0x8000))
		return fix8_overflow;
	
	return sum;
}

fix8_t fix8_sub(fix8_t a, fix8_t b)
{
	uint16_t _a = a, _b = b;
	uint16_t diff = _a - _b;

	// Overflow can only happen if sign of a != sign of b, and then
	// it causes sign of diff != sign of a.
	if (((_a ^ _b) & 0x8000) && ((_a ^ diff) & 0x8000))
		return fix8_overflow;
	
	return diff;
}

/* Saturating arithmetic */
fix8_t fix8_sadd(fix8_t a, fix8_t b)
{
	fix8_t result = fix8_add(a, b);

	if (result == fix8_overflow)
		return (a >= 0) ? fix8_maximum : fix8_minimum;

	return result;
}	

fix8_t fix8_ssub(fix8_t a, fix8_t b)
{
	fix8_t result = fix8_sub(a, b);

	if (result == fix8_overflow)
		return (a >= 0) ? fix8_maximum : fix8_minimum;

	return result;
}
#endif
/*
fix8_t fix8_mul(fix8_t inArg0, fix8_t inArg1)
{
	int32_t product = (int32_t)inArg0 * inArg1;
	
#ifndef FIXMATH_NO_OVERFLOW
	// The upper 9 bits should all be the same (the sign).
	uint16_t upper = (product >> 23);
#endif
	
	if (product < 0)
	{
#ifndef FIXMATH_NO_OVERFLOW
		if (~upper)
				return fix8_overflow;
#endif
		
#ifndef FIXMATH_NO_ROUNDING
		// This adjustment is required in order to round -1/2 correctly
		product--;
#endif
	}
	else
	{
#ifndef FIXMATH_NO_OVERFLOW
		if (upper)
				return fix8_overflow;
#endif
	}
	
#ifdef FIXMATH_NO_ROUNDING
	return product >> 8;
#else
	fix8_t result = product >> 8;
	result += (product & 0x80) >> 7;
	
	return result;
#endif
}
*/

/* 8-bit implementation of fix16_mul. Fastest on e.g. Atmel AVR.
 * Uses 8*8->16bit multiplications, and also skips any bytes that
 * are zero.
 */
// TODO: See also http://mekonik.wordpress.com/2009/03/18/arduino-avr-gcc-multiplication/
#if defined(FIXMATH_OPTIMIZE_8BIT)
fix8_t fix8_mul(fix8_t inArg0, fix8_t inArg1)
{
	uint16_t _a = (inArg0 >= 0) ? inArg0 : (-inArg0);
	uint16_t _b = (inArg1 >= 0) ? inArg1 : (-inArg1);
	
	uint8_t va[2] = {_a, (_a >> 8)};
	uint8_t vb[2] = {_b, (_b >> 8)};
	
	uint16_t low = 0;
	
	// Result column i depends on va[0..i] and vb[i..0]

	#ifndef FIXMATH_NO_OVERFLOW
	// i = 6
	if (va[1] && vb[1]) return fix8_overflow;
	#endif

  // x * y = 65536 * x1 * y1 + 256 * x1 * y0 + 256 * x0 * y1 + x0 * y0
  // 0x0200 * 0x0300 = 0x0600
  
	// i = 1
	if (va[0] && vb[1]) low += (uint16_t)va[0] * vb[1];
	if (va[1] && vb[0]) low += (uint16_t)va[1] * vb[0];
	low <<= 8;
	
	// i = 0
	if (va[0] && vb[0]) low += (uint16_t)va[0] * vb[0];
	
	#ifndef FIXMATH_NO_ROUNDING
	low += 0x80;
	#endif
	low >>= 8;
  
	#ifndef FIXMATH_NO_OVERFLOW
	if (low & 0x8000)
		return fix8_overflow;
	#endif
	
	fix8_t result = low;
	
	/* Figure out the sign of result */
	if ((inArg0 >= 0) != (inArg1 >= 0))
	{
		result = -result;
	}
	
	return result;
}
#endif


#ifndef FIXMATH_NO_OVERFLOW
/* Wrapper around fix8_mul to add saturating arithmetic. */
fix8_t fix8_smul(fix8_t inArg0, fix8_t inArg1)
{
	fix8_t result = fix8_mul(inArg0, inArg1);
	
	if (result == fix8_overflow)
	{
		if ((inArg0 >= 0) == (inArg1 >= 0))
			return fix8_maximum;
		else
			return fix8_minimum;
	}
	
	return result;
}
#endif

/* Alternative 32-bit implementation of fix16_div. Fastest on e.g. Atmel AVR.
 * This does the division manually, and is therefore good for processors that
 * do not have hardware division.
 */
#if defined(FIXMATH_OPTIMIZE_8BIT)
fix8_t fix8_div(fix8_t a, fix8_t b)
{
	// This uses the basic binary restoring division algorithm.
	// It appears to be faster to do the whole division manually than
	// trying to compose a 64-bit divide out of 32-bit divisions on
	// platforms without hardware divide.
	
	if (b == 0)
		return fix8_minimum;
	
	uint16_t remainder = (a >= 0) ? a : (-a);
	uint16_t divider = (b >= 0) ? b : (-b);

	uint16_t quotient = 0;
	uint16_t bit = 0x100;
	
	/* The algorithm requires D >= R */
	while (divider < remainder)
	{
		divider <<= 1;
		bit <<= 1;
	}
	
	#ifndef FIXMATH_NO_OVERFLOW
	if (!bit)
		return fix8_overflow;
	#endif
	
	if (divider & 0x8000)
	{
		// Perform one step manually to avoid overflows later.
		// We know that divider's bottom bit is 0 here.
		if (remainder >= divider)
		{
				quotient |= bit;
				remainder -= divider;
		}
		divider >>= 1;
		bit >>= 1;
	}
	
	/* Main division loop */
	while (bit && remainder)
	{
		if (remainder >= divider)
		{
				quotient |= bit;
				remainder -= divider;
		}
		
		remainder <<= 1;
		bit >>= 1;
	}	 
			
	#ifndef FIXMATH_NO_ROUNDING
	if (remainder >= divider)
	{
		quotient++;
	}
	#endif
	
	fix8_t result = quotient;
	
	/* Figure out the sign of result */
	if ((a ^ b) & 0x8000)
	{
		#ifndef FIXMATH_NO_OVERFLOW
		if (result == fix8_minimum)
				return fix8_overflow;
		#endif
		
		result = -result;
	}
	
	return result;
}
#endif

#ifndef FIXMATH_NO_OVERFLOW
/* Wrapper around fix8_div to add saturating arithmetic. */
fix8_t fix8_sdiv(fix8_t inArg0, fix8_t inArg1)
{
	fix8_t result = fix8_div(inArg0, inArg1);
	
	if (result == fix8_overflow)
	{
		if ((inArg0 >= 0) == (inArg1 >= 0))
			return fix8_maximum;
		else
			return fix8_minimum;
	}
	
	return result;
}
#endif

fix8_t fix8_mod(fix8_t x, fix8_t y)
{
	#ifdef FIXMATH_OPTIMIZE_8BIT
		/* The reason we do this, rather than use a modulo operator
		 * is that if you don't have a hardware divider, this will result
		 * in faster operations when the angles are close to the bounds. 
		 */
		while(x >=  y) x -= y;
		while(x <= -y) x += y;
	#else
		/* Note that in C90, the sign of result of the modulo operation is
		 * undefined. in C99, it's the same as the dividend (aka numerator).
		 */
		x %= y;
	#endif

	return x;
}


#ifndef FIXMATH_NO_64BIT

fix8_t fix8_lerp8(fix8_t inArg0, fix8_t inArg1, uint8_t inFract)
{
	int64_t tempOut = int64_mul_i32_i32(inArg0, ((1 << 8) - inFract));
	tempOut = int64_add(tempOut, int64_mul_i32_i32(inArg1, inFract));
	tempOut = int64_shift(tempOut, -8);
	return (fix8_t)int64_lo(tempOut);
}

fix8_t fix8_lerp16(fix8_t inArg0, fix8_t inArg1, uint16_t inFract)
{
	int64_t tempOut = int64_mul_i32_i32(inArg0, (((int32_t)1 << 16) - inFract));
	tempOut = int64_add(tempOut, int64_mul_i32_i32(inArg1, inFract));
	tempOut = int64_shift(tempOut, -16);
	return (fix8_t)int64_lo(tempOut);
}

fix8_t fix8_lerp32(fix8_t inArg0, fix8_t inArg1, uint32_t inFract)
{
	int64_t tempOut;
	tempOut  = ((int64_t)inArg0 * (0 - inFract));
	tempOut	+= ((int64_t)inArg1 * inFract);
	tempOut >>= 32;
	return (fix8_t)tempOut;
}
#endif
