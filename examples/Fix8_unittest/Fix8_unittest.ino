#include <fix8.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "unittests.h"

static int status = 0;

const double f8max = Fix8(fix8_maximum);
const double f8min = Fix8(fix8_minimum);

const fix8_t testcases[] = {
  // Small numbers
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  -1, -2, -3, -4, -5, -6, -7, -8, -9, -10,
  
  // Integer numbers
  0x100, -0x100, 0x200, -0x200, 0x300, -0x300,
  0x400, -0x400, 0x500, -0x500, 0x600, -0x600,
  
  // Fractions (1/2, 1/4, 1/8)
  0x80, -0x80, 0x40, -0x40, 0x20, -0x20,
  
  // Problematic carry
  0xFF, -0xFF, 0x1FF, -0x1FF, 0x3FF, -0x3FF,
  
  // Smallest and largest values
  0x7FFF, 0x8000,
  
  // Large random numbers
  831858892/0x10000,  574794913/0x10000, 2147272293/0x10000, -469161054/0x10000, -961611615/0x10000,
  1841960234/0x10000, 1992698389/0x10000, 520485404/0x10000, 560523116/0x10000, -2094993050/0x10000,
  -876897543/0x10000, -67813629/0x10000, 2146227091/0x10000, 509861939/0x10000, -1073573657/0x10000,

  // Small random numbers
  -14985/0x100, 30520/0x100, -83587/0x100, 41129/0x100, 42137/0x100, 58537/0x100, -2259/0x100, 84142/0x100,
  -28283/0x100, 90914/0x100, 19865/0x100, 33191/0x100, 81844/0x100, -66273/0x100, -63215/0x100, -44459/0x100,
  -11326/0x100, 84295/0x100, 47515/0x100, -39324/0x100,
  
  // Tiny random numbers
  -171/0x10, -359/0x10, 491/0x10, 844/0x10, 158/0x10, -413/0x10, -422/0x10, -737/0x10, -575/0x10, -330/0x10,
  -376/0x10, 435/0x10, -311/0x10, 116/0x10, 715/0x10, -1024/0x10, -487/0x10, 59/0x10, 724/0x10, 993/0x10
};

#define TESTCASES_COUNT (sizeof(testcases)/sizeof(testcases[0]))

#define delta(a,b) (((a)>=(b)) ? (a)-(b) : (b)-(a))

#ifdef FIXMATH_NO_ROUNDING
const Fix8 max_delta(0.3);
#else
const Fix8 max_delta(0.3);
#endif 

void test_multBasic( void )
{
  COMMENT("Testing basic multiplication");
  TEST(Fix8(int8_t(5)) * Fix8(int8_t(5)) == Fix8(int8_t(25)));
  TEST(Fix8(int8_t(-5)) * Fix8(int8_t(5)) == Fix8(int8_t(-25)));
  TEST(Fix8(int8_t(-5)) * Fix8(int8_t(-5)) == Fix8(int8_t(25)));
  TEST(Fix8(int8_t(5)) * Fix8(int8_t(-5)) == Fix8(int8_t(-25)));
}

void test_multRound( void )
{
#ifndef FIXMATH_NO_ROUNDING
  COMMENT("Testing multiplication rounding corner cases");
  TEST(fix8_mul(0, 10) == 0);
  TEST(fix8_mul(2, 0x80) == 1);
  TEST(fix8_mul(-2, 0x80) == -1);
  TEST(fix8_mul(3, 0x80) == 2);
  TEST(fix8_mul(-3, 0x80) == -2);
  TEST(fix8_mul(2, 0x7F) == 1);
  TEST(fix8_mul(-2, 0x7F) == -1);
  TEST(fix8_mul(2, 0x81) == 1);
  TEST(fix8_mul(-2, 0x81) == -1);
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
      Fix8 a = testcases[i];
      Fix8 b = testcases[j];
      Fix8 result = a * b;
      
      double fa = a;
      double fb = b;
      double fresult = fa * fb;
      
      if (delta(Fix8(fresult), result) > max_delta)
      {
        if ((fa * fb > f8max) || (fa * fb < f8min))
        {
#ifndef FIXMATH_NO_OVERFLOW
          if (result != fix8_overflow)
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
  TEST(Fix8(int8_t(15)) / Fix8(int8_t(5)) == Fix8(int8_t(3)));
  TEST(Fix8(int8_t(-15)) / Fix8(int8_t(5)) == Fix8(int8_t(-3)));
  TEST(Fix8(int8_t(-15)) / Fix8(int8_t(-5)) == Fix8(int8_t(3)));
  TEST(Fix8(int8_t(15)) / Fix8(int8_t(-5)) == Fix8(int8_t(-3)));
}

void test_divRound( void )
{
#ifndef FIXMATH_NO_ROUNDING
  COMMENT("Testing division rounding corner cases");
  TEST(fix8_div(0, 10) == 0);
  TEST(fix8_div(1, fix8_from_int(2)) == 1);
  TEST(fix8_div(-1, fix8_from_int(2)) == -1);
  TEST(fix8_div(1, fix8_from_int(-2)) == -1);
  TEST(fix8_div(-1, fix8_from_int(-2)) == 1);
  TEST(fix8_div(3, fix8_from_int(2)) == 2);
  TEST(fix8_div(-3, fix8_from_int(2)) == -2);
  TEST(fix8_div(3, fix8_from_int(-2)) == -2);
  TEST(fix8_div(-3, fix8_from_int(-2)) == 2);
  TEST(fix8_div(2, 0x7F) == 4);
  TEST(fix8_div(-2, 0x7F) == -4);
  TEST(fix8_div(2, 0x81) == 4);
  TEST(fix8_div(-2, 0x81) == -4);
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
      Fix8 a = testcases[i];
      Fix8 b = testcases[j];
      
      // We don't require a solution for /0 :)
      if (b == 0) continue;
      
      Fix8 result = a / b;
      
      double fa = a;
      double fb = b;
      double fresult = fa / fb;
      
      if (delta(Fix8(fresult), result) > max_delta)
      {
        if (((fa / fb) > f8max) || ((fa / fb) < f8min))
        {
#ifndef FIXMATH_NO_OVERFLOW
          if (result != fix8_overflow)
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
      Fix8 a = testcases[i];
      Fix8 b = testcases[j];
      Fix8 result = a + b;
      
      double fa = a;
      double fb = b;
      double fresult = fa + fb;
      
      if (delta(Fix8(fresult), result) > max_delta)
      {
        if ((fa + fb > f8max) || (fa + fb < f8min))
        {
#ifndef FIXMATH_NO_OVERFLOW
          if (result != fix8_overflow)
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
      Fix8 a = testcases[i];
      Fix8 b = testcases[j];
      Fix8 result = a - b;
      
      double fa = a;
      double fb = b;
      double fresult = fa - fb;
      
      if (delta(Fix8(fresult), result) > max_delta)
      {
        if ((fa - fb > f8max) || (fa - fb < f8min))
        {
#ifndef FIXMATH_NO_OVERFLOW
          if (result != fix8_overflow)
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

  if (status != 0)
    Serial.println("\n\nSome tests FAILED!");

  COMMENT("Test finished");
  while (1) {};
}

void loop()
{
}
