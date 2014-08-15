#include <fix16.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "unittests.h"

static int status = 0;

const double f16max = Fix16(fix16_maximum);
const double f16min = Fix16(fix16_minimum);

const fix16_t testcases[] = {
  // Small numbers
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  -1, -2, -3, -4, -5, -6, -7, -8, -9, -10,
  
  // Integer numbers
  0x10000, -0x10000, 0x20000, -0x20000, 0x30000, -0x30000,
  0x40000, -0x40000, 0x50000, -0x50000, 0x60000, -0x60000,
  
  // Fractions (1/2, 1/4, 1/8)
  0x8000, -0x8000, 0x4000, -0x4000, 0x2000, -0x2000,
  
  // Problematic carry
  0xFFFF, -0xFFFF, 0x1FFFF, -0x1FFFF, 0x3FFFF, -0x3FFFF,
  
  // Smallest and largest values
  0x7FFFFFFF, 0x80000000,
  
  // Large random numbers
  831858892, 574794913, 2147272293, -469161054, -961611615,
  1841960234, 1992698389, 520485404, 560523116, -2094993050,
  -876897543, -67813629, 2146227091, 509861939, -1073573657,
  
  // Small random numbers
  -14985, 30520, -83587, 41129, 42137, 58537, -2259, 84142,
  -28283, 90914, 19865, 33191, 81844, -66273, -63215, -44459,
  -11326, 84295, 47515, -39324,
  
  // Tiny random numbers
  -171, -359, 491, 844, 158, -413, -422, -737, -575, -330,
  -376, 435, -311, 116, 715, -1024, -487, 59, 724, 993
};

#define TESTCASES_COUNT (sizeof(testcases)/sizeof(testcases[0]))

#define delta(a,b) (((a)>=(b)) ? (a)-(b) : (b)-(a))

#ifdef FIXMATH_NO_ROUNDING
const Fix16 max_delta(0.003);
#else
const Fix16 max_delta(0.003);
#endif 

void test_multBasic( void )
{
  COMMENT("Testing basic multiplication");
  TEST(Fix16(5) * Fix16(5) == Fix16(25));
  TEST(Fix16(-5) * Fix16(5) == Fix16(-25));
  TEST(Fix16(-5) * Fix16(-5) == Fix16(25));
  TEST(Fix16(5) * Fix16(-5) == Fix16(-25));
}

void test_multRound( void )
{
#ifndef FIXMATH_NO_ROUNDING
  COMMENT("Testing multiplication rounding corner cases");
  TEST(fix16_mul(0, 10) == 0);
  TEST(fix16_mul(2, 0x8000) == 1);
  TEST(fix16_mul(-2, 0x8000) == -1);
  TEST(fix16_mul(3, 0x8000) == 2);
  TEST(fix16_mul(-3, 0x8000) == -2);
  TEST(fix16_mul(2, 0x7FFF) == 1);
  TEST(fix16_mul(-2, 0x7FFF) == -1);
  TEST(fix16_mul(2, 0x8001) == 1);
  TEST(fix16_mul(-2, 0x8001) == -1);
#endif
}

void test_multTestcases( void )
{
  unsigned int i, j;
  int failures = 0;
  COMMENT("Running testcases for multiplication");

  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    for (j = 0; j < TESTCASES_COUNT; j++)
    {
      Fix16 a = testcases[i];
      Fix16 b = testcases[j];
      Fix16 result = a * b;
      
      double fa = a;
      double fb = b;
      double fresult = fa * fb;
      
      if (delta(Fix16(fresult), result) > max_delta)
      {
        if ((fa * fb > f16max) || (fa * fb < f16min))
        {
#ifndef FIXMATH_NO_OVERFLOW
          if (result != fix16_overflow)
          {
            LOG_OVERFLOW_NOT_DETECTED(a, b, "*");
            failures++;
          }
#endif
          // Legitimate overflow
          continue;
        }
        LOG_NEWLINE
        LOG_EXPRESSION(a, b, result, "*");
        LOG_EXPRESSION(fa, fb, fresult, "*");
        failures++;
      }
    }
  }
  
  TEST(failures == 0);
}


void test_divBasic( void )
{
  COMMENT("Testing basic division");
  TEST(Fix16(15) / Fix16(5) == Fix16(3));
  TEST(Fix16(-15) / Fix16(5) == Fix16(-3));
  TEST(Fix16(-15) / Fix16(-5) == Fix16(3));
  TEST(Fix16(15) / Fix16(-5) == Fix16(-3));
}

void test_divRound( void )
{
#ifndef FIXMATH_NO_ROUNDING
  COMMENT("Testing division rounding corner cases");
  TEST(fix16_div(0, 10) == 0);
  TEST(fix16_div(1, fix16_from_int(2)) == 1);
  TEST(fix16_div(-1, fix16_from_int(2)) == -1);
  TEST(fix16_div(1, fix16_from_int(-2)) == -1);
  TEST(fix16_div(-1, fix16_from_int(-2)) == 1);
  TEST(fix16_div(3, fix16_from_int(2)) == 2);
  TEST(fix16_div(-3, fix16_from_int(2)) == -2);
  TEST(fix16_div(3, fix16_from_int(-2)) == -2);
  TEST(fix16_div(-3, fix16_from_int(-2)) == 2);
  TEST(fix16_div(2, 0x7FFF) == 4);
  TEST(fix16_div(-2, 0x7FFF) == -4);
  TEST(fix16_div(2, 0x8001) == 4);
  TEST(fix16_div(-2, 0x8001) == -4);
#endif
}

void test_divTestcases( void )
{
  unsigned int i, j;
  int failures = 0;
  COMMENT("Running testcases for division");
  
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    for (j = 0; j < TESTCASES_COUNT; j++)
    {
      Fix16 a = testcases[i];
      Fix16 b = testcases[j];
      
      // We don't require a solution for /0 :)
      if (b == 0) continue;
      
      Fix16 result = a / b;
      
      double fa = a;
      double fb = b;
      double fresult = fa / fb;
      
      if (delta(Fix16(fresult), result) > max_delta)
      {
        if (((fa / fb) > f16max) || ((fa / fb) < f16min))
        {
#ifndef FIXMATH_NO_OVERFLOW
          if (result != fix16_overflow)
          {
            LOG_OVERFLOW_NOT_DETECTED(a, b, "/");
            failures++;
          }
#endif
          // Legitimate overflow
          continue;
        }
        LOG_NEWLINE
        LOG_EXPRESSION(a, b, result, "/");
        LOG_EXPRESSION(fa, fb, fresult, "/");
        failures++;
      }
    }
  }
  
  TEST(failures == 0);
}

void test_addTestcases( void )
{
  unsigned int i, j;
  int failures = 0;
  COMMENT("Running testcases for addition");
  
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    for (j = 0; j < TESTCASES_COUNT; j++)
    {
      Fix16 a = testcases[i];
      Fix16 b = testcases[j];
      Fix16 result = a + b;
      
      double fa = a;
      double fb = b;
      double fresult = fa + fb;
      
      if (delta(Fix16(fresult), result) > max_delta)
      {
        if ((fa + fb > f16max) || (fa + fb < f16min))
        {
#ifndef FIXMATH_NO_OVERFLOW
          if (result != fix16_overflow)
          {
    Serial.println(double(result), FLOAT_DECS);
            LOG_OVERFLOW_NOT_DETECTED(a, b, "+");
            failures++;
          }
#endif
          // Legitimate overflow
          continue;
        }
        LOG_NEWLINE
        LOG_EXPRESSION(a, b, result, "+");
        LOG_EXPRESSION(fa, fb, fresult, "+");
        failures++;
      }
    }
  }
  
  TEST(failures == 0);
}

void test_subTestcases( void )
{
  unsigned int i, j;
  int failures = 0;
  COMMENT("Running testcases for subtraction");
  
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    for (j = 0; j < TESTCASES_COUNT; j++)
    {
      Fix16 a = testcases[i];
      Fix16 b = testcases[j];
      Fix16 result = a - b;
      
      double fa = a;
      double fb = b;
      double fresult = fa - fb;
      
      if (delta(Fix16(fresult), result) > max_delta)
      {
        if ((fa - fb > f16max) || (fa - fb < f16min))
        {
#ifndef FIXMATH_NO_OVERFLOW
          if (result != fix16_overflow)
          {
            LOG_OVERFLOW_NOT_DETECTED(a, b, "-");
            failures++;
          }
#endif
          // Legitimate overflow
          continue;
        }
        LOG_NEWLINE
        LOG_EXPRESSION(a, b, result, "-");
        LOG_EXPRESSION(fa, fb, fresult, "-");
        failures++;
      }
    }
  }
  
  TEST(failures == 0);
}


void test_sqrtBasic( void )
{
  COMMENT("Testing basic square roots");
  TEST(Fix16(16).sqrt() == Fix16(4));
  TEST(Fix16(100).sqrt() == Fix16(10));
  TEST(Fix16(1).sqrt() == Fix16(1));
}

void test_sqrtRound( void )
{
#ifndef FIXMATH_NO_ROUNDING
  COMMENT("Testing square root rounding corner cases");
  TEST(fix16_sqrt(214748302) == 3751499);
  TEST(fix16_sqrt(214748303) == 3751499);
  TEST(fix16_sqrt(214748359) == 3751499);
  TEST(fix16_sqrt(214748360) == 3751500);
#endif
}

void test_sqrtTestcases( void )
{
  unsigned int i;
  int failures = 0;
  COMMENT("Running test cases for square root");
  
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    Fix16 a = testcases[i];
    if (a < Fix16(0)) continue;
    Fix16 result = a.sqrt();
    
    double fa = double(a);
    double fresult = sqrt(fa);
    
    if (delta(Fix16(fresult), result) > max_delta)
    {
      LOG_NEWLINE
//      Serial.print(F("Delta "));
//      Serial.println(double(delta(Fix16(fresult), result)), FLOAT_DECS);

      Serial.print(F("Fix16.sqrt "));
      Serial.print(double(a), FLOAT_DECS);
      Serial.print(F(" = "));
      Serial.println(double(result), FLOAT_DECS);

      Serial.print(F("sqrt "));
      Serial.print(fa, FLOAT_DECS);
      Serial.print(F(" = "));
      Serial.println(fresult, FLOAT_DECS);
      failures++;
    }
  }
  
  TEST(failures == 0);
}


void setup()
{
  Serial.begin(115200);

  test_multBasic();
  test_multRound();
  test_multTestcases();
  test_divBasic();
  test_divRound();
  test_divTestcases();
  test_addTestcases();
  test_subTestcases();
  test_sqrtBasic();
  test_sqrtRound();
  test_sqrtTestcases();

  if (status != 0)
    Serial.println("\n\nSome tests FAILED!");

  COMMENT("Test finished");
  while (1) {};
}

void loop()
{
}
