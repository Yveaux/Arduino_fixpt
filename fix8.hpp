#ifndef __libfixmath_fix8_hpp__
#define __libfixmath_fix8_hpp__

#include "fix8.h"

class Fix8 {
	public:
		fix8_t value;

		Fix8() { value = 0; }
		Fix8(const Fix8 &inValue)  { value = inValue.value;            }
		Fix8(const fix8_t inValue) { value = inValue;                  }
		Fix8(const float inValue)  { value = fix8_from_float(inValue); }
		Fix8(const double inValue) { value = fix8_from_dbl(inValue);   }
		Fix8(const int8_t inValue) { value = fix8_from_int(inValue);   }

		operator fix8_t() const { return value;                }
		operator double() const { return fix8_to_dbl(value);   }
		operator float()  const { return fix8_to_float(value); }
		operator int8_t() const { return fix8_to_int(value);   }
		operator bool()   const { return bool(value);          }

		Fix8 & operator=(const Fix8 &rhs)  { value = rhs.value;            return *this; }
		Fix8 & operator=(const fix8_t rhs) { value = rhs;                  return *this; }
		Fix8 & operator=(const double rhs) { value = fix8_from_dbl(rhs);   return *this; }
		Fix8 & operator=(const float rhs)  { value = fix8_from_float(rhs); return *this; }
		Fix8 & operator=(const int8_t rhs) { value = fix8_from_int(rhs);   return *this; }

		Fix8 & operator+=(const Fix8 &rhs)  { value = fix8_add(value, rhs.value);            return *this; }
		Fix8 & operator+=(const fix8_t rhs) { value = fix8_add(value, rhs);                  return *this; }
		Fix8 & operator+=(const double rhs) { value = fix8_add(value, fix8_from_dbl(rhs));   return *this; }
		Fix8 & operator+=(const float rhs)  { value = fix8_add(value, fix8_from_float(rhs)); return *this; }
		Fix8 & operator+=(const int8_t rhs) { value = fix8_add(value, fix8_from_int(rhs));   return *this; }

		Fix8 & operator-=(const Fix8 &rhs)  { value = fix8_sub(value, rhs.value); return *this; }
		Fix8 & operator-=(const fix8_t rhs) { value = fix8_sub(value, rhs); return *this; }
		Fix8 & operator-=(const double rhs) { value = fix8_sub(value, fix8_from_dbl(rhs)); return *this; }
		Fix8 & operator-=(const float rhs)  { value = fix8_sub(value, fix8_from_float(rhs)); return *this; }
		Fix8 & operator-=(const int8_t rhs) { value = fix8_sub(value, fix8_from_int(rhs)); return *this; }

		Fix8 & operator*=(const Fix8 &rhs)  { value = fix8_mul(value, rhs.value); return *this; }
		Fix8 & operator*=(const fix8_t rhs) { value = fix8_mul(value, rhs); return *this; }
		Fix8 & operator*=(const double rhs) { value = fix8_mul(value, fix8_from_dbl(rhs)); return *this; }
		Fix8 & operator*=(const float rhs)  { value = fix8_mul(value, fix8_from_float(rhs)); return *this; }
		Fix8 & operator*=(const int8_t rhs) { value = fix8_mul(value, fix8_from_int(rhs)); return *this; }

		Fix8 & operator/=(const Fix8 &rhs)  { value = fix8_div(value, rhs.value); return *this; }
		Fix8 & operator/=(const fix8_t rhs) { value = fix8_div(value, rhs); return *this; }
		Fix8 & operator/=(const double rhs) { value = fix8_div(value, fix8_from_dbl(rhs)); return *this; }
		Fix8 & operator/=(const float rhs)  { value = fix8_div(value, fix8_from_float(rhs)); return *this; }
		Fix8 & operator/=(const int8_t rhs) { value = fix8_div(value, fix8_from_int(rhs)); return *this; }

		const Fix8 operator+(const Fix8 &other)  const { Fix8 ret = *this; ret += other; return ret; }
		const Fix8 operator+(const fix8_t other) const { Fix8 ret = *this; ret += other; return ret; }
		const Fix8 operator+(const double other) const { Fix8 ret = *this; ret += other; return ret; }
		const Fix8 operator+(const float other)  const { Fix8 ret = *this; ret += other; return ret; }
		const Fix8 operator+(const int8_t other) const { Fix8 ret = *this; ret += other; return ret; }

#ifndef FIXMATH_NO_OVERFLOW
		const Fix8 sadd(const Fix8 &other)  const { Fix8 ret = fix8_sadd(value, other.value);            return ret; }
		const Fix8 sadd(const fix8_t other) const { Fix8 ret = fix8_sadd(value, other);                  return ret; }
		const Fix8 sadd(const double other) const { Fix8 ret = fix8_sadd(value, fix8_from_dbl(other));   return ret; }
		const Fix8 sadd(const float other)  const { Fix8 ret = fix8_sadd(value, fix8_from_float(other)); return ret; }
		const Fix8 sadd(const int8_t other) const { Fix8 ret = fix8_sadd(value, fix8_from_int(other));   return ret; }
#endif

		const Fix8 operator-(const Fix8 &other)  const  { Fix8 ret = *this; ret -= other; return ret; }
		const Fix8 operator-(const fix8_t other) const { Fix8 ret = *this; ret -= other; return ret; }
		const Fix8 operator-(const double other) const  { Fix8 ret = *this; ret -= other; return ret; }
		const Fix8 operator-(const float other)  const   { Fix8 ret = *this; ret -= other; return ret; }
		const Fix8 operator-(const int8_t other) const { Fix8 ret = *this; ret -= other; return ret; }

#ifndef FIXMATH_NO_OVERFLOW
		const Fix8 ssub(const Fix8 &other)  const { Fix8 ret = fix8_sadd(value, -other.value);            return ret; }
		const Fix8 ssub(const fix8_t other) const { Fix8 ret = fix8_sadd(value, -other);                  return ret; }
		const Fix8 ssub(const double other) const { Fix8 ret = fix8_sadd(value, -fix8_from_dbl(other));   return ret; }
		const Fix8 ssub(const float other)  const { Fix8 ret = fix8_sadd(value, -fix8_from_float(other)); return ret; }
		const Fix8 ssub(const int8_t other) const { Fix8 ret = fix8_sadd(value, -fix8_from_int(other));   return ret; }
#endif

		const Fix8 operator*(const Fix8 &other)  const { Fix8 ret = *this; ret *= other; return ret; }
		const Fix8 operator*(const fix8_t other) const { Fix8 ret = *this; ret *= other; return ret; }
		const Fix8 operator*(const double other) const { Fix8 ret = *this; ret *= other; return ret; }
		const Fix8 operator*(const float other)  const { Fix8 ret = *this; ret *= other; return ret; }
		const Fix8 operator*(const int8_t other) const { Fix8 ret = *this; ret *= other; return ret; }

#ifndef FIXMATH_NO_OVERFLOW
		const Fix8 smul(const Fix8 &other)  const { Fix8 ret = fix8_smul(value, other.value);            return ret; }
		const Fix8 smul(const fix8_t other) const { Fix8 ret = fix8_smul(value, other);                  return ret; }
		const Fix8 smul(const double other) const { Fix8 ret = fix8_smul(value, fix8_from_dbl(other));   return ret; }
		const Fix8 smul(const float other)  const { Fix8 ret = fix8_smul(value, fix8_from_float(other)); return ret; }
		const Fix8 smul(const int8_t other) const { Fix8 ret = fix8_smul(value, fix8_from_int(other));   return ret; }
#endif

		const Fix8 operator/(const Fix8 &other)  const { Fix8 ret = *this; ret /= other; return ret; }
		const Fix8 operator/(const fix8_t other) const { Fix8 ret = *this; ret /= other; return ret; }
		const Fix8 operator/(const double other) const { Fix8 ret = *this; ret /= other; return ret; }
		const Fix8 operator/(const float other)  const { Fix8 ret = *this; ret /= other; return ret; }
		const Fix8 operator/(const int8_t other) const { Fix8 ret = *this; ret /= other; return ret; }

#ifndef FIXMATH_NO_OVERFLOW
		const Fix8 sdiv(const Fix8 &other)  const { Fix8 ret = fix8_sdiv(value, other.value);            return ret; }
		const Fix8 sdiv(const fix8_t other) const { Fix8 ret = fix8_sdiv(value, other);                  return ret; }
		const Fix8 sdiv(const double other) const { Fix8 ret = fix8_sdiv(value, fix8_from_dbl(other));   return ret; }
		const Fix8 sdiv(const float other)  const { Fix8 ret = fix8_sdiv(value, fix8_from_float(other)); return ret; }
		const Fix8 sdiv(const int8_t other) const { Fix8 ret = fix8_sdiv(value, fix8_from_int(other));   return ret; }
#endif

		const int operator==(const Fix8 &other)  const { return (value == other.value);            }
		const int operator==(const fix8_t other) const { return (value == other);                  }
		const int operator==(const double other) const { return (value == fix8_from_dbl(other));   }
		const int operator==(const float other)  const { return (value == fix8_from_float(other)); }
		const int operator==(const int8_t other) const { return (value == fix8_from_int(other));   }

		const int operator!=(const Fix8 &other)  const { return (value != other.value);            }
		const int operator!=(const fix8_t other) const { return (value != other);                  }
		const int operator!=(const double other) const { return (value != fix8_from_dbl(other));   }
		const int operator!=(const float other)  const { return (value != fix8_from_float(other)); }
		const int operator!=(const int8_t other) const { return (value != fix8_from_int(other));   }

		const int operator<=(const Fix8 &other)  const { return (value <= other.value);            }
		const int operator<=(const fix8_t other) const { return (value <= other);                  }
		const int operator<=(const double other) const { return (value <= fix8_from_dbl(other));   }
		const int operator<=(const float other)  const { return (value <= fix8_from_float(other)); }
		const int operator<=(const int8_t other) const { return (value <= fix8_from_int(other));   }

		const int operator>=(const Fix8 &other)  const { return (value >= other.value);            }
		const int operator>=(const fix8_t other) const { return (value >= other);                  }
		const int operator>=(const double other) const { return (value >= fix8_from_dbl(other));   }
		const int operator>=(const float other)  const { return (value >= fix8_from_float(other)); }
		const int operator>=(const int8_t other) const { return (value >= fix8_from_int(other));   }

		const int operator< (const Fix8 &other)  const { return (value <  other.value);            }
		const int operator< (const fix8_t other) const { return (value <  other);                  }
		const int operator< (const double other) const { return (value <  fix8_from_dbl(other));   }
		const int operator< (const float other)  const { return (value <  fix8_from_float(other)); }
		const int operator< (const int8_t other) const { return (value <  fix8_from_int(other));   }

		const int operator> (const Fix8 &other)  const { return (value >  other.value);            }
		const int operator> (const fix8_t other) const { return (value >  other);                  }
		const int operator> (const double other) const { return (value >  fix8_from_dbl(other));   }
		const int operator> (const float other)  const { return (value >  fix8_from_float(other)); }
		const int operator> (const int8_t other) const { return (value >  fix8_from_int(other));   }
/*
		Fix8  sin() { return Fix8(fix8_sin(value));  }
		Fix8  cos() { return Fix8(fix8_cos(value));  }
		Fix8  tan() { return Fix8(fix8_tan(value));  }
		Fix8 asin() { return Fix8(fix8_asin(value)); }
		Fix8 acos() { return Fix8(fix8_acos(value)); }
		Fix8 atan() { return Fix8(fix8_atan(value)); }
		Fix8 atan2(const Fix8 &inY) { return Fix8(fix8_atan2(value, inY.value)); }
		Fix8 sqrt() { return Fix8(fix8_sqrt(value)); }
*/
};

#endif
