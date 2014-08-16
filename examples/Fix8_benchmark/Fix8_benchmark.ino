//#define NO_DOUBLE
#define NO_FIX8

#if !defined(NO_DOUBLE) && !defined(NO_FIX8)
#include <stdio.h>
#endif

#ifndef NO_FIX8
#include <fix8.h>
#endif
#ifndef NO_DOUBLE
#include <math.h>
#endif

#define NUM_RUNS   (5)

#define COMMENT(x) Serial.println(F("\n----" x "----"));

const int16_t testcases[] = {
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

//__attribute__((optimize("O2")))

// Volatile global variable which tests will assign values to, to assure compiler will not
// remove calculation statement as it now has side-effects.
static volatile bool f;

template<typename T> void test_multTestcases( void )
{
  unsigned int i, j;
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    for (j = 0; j < TESTCASES_COUNT; j++)
    {
      T a = testcases[i];
      T b = testcases[j];
      f = a * b;
    }
  }
}

template<typename T> void test_divTestcases( void )
{
  unsigned int i, j;
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    for (j = 0; j < TESTCASES_COUNT; j++)
    {
      T a = testcases[i];
      T b = testcases[j];
      // We don't require a solution for /0 :)
      if (b == T(0)) continue;
      f = a / b;
    }
  }
}

template<typename T> void test_addTestcases( void )
{
  unsigned int i, j;
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    for (j = 0; j < TESTCASES_COUNT; j++)
    {
      T a = testcases[i];
      T b = testcases[j];
      f = a + b;
    }
  }
}

template<typename T> void test_subTestcases( void )
{
  unsigned int i, j;
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    for (j = 0; j < TESTCASES_COUNT; j++)
    {
      T a = testcases[i];
      T b = testcases[j];
      f = a - b;
    }
  }
}

#define TIMED_EXEC(func,delta,runs)  \
{                                    \
  unsigned long t0 = micros();       \
  for (uint8_t i = 0; i < runs; ++i) \
    func();                          \
  delta = micros() - t0;             \
}

void setup()
{
#if !defined(NO_DOUBLE) && !defined(NO_FIX8)
  Serial.begin(115200);

  COMMENT("Running testcases for multiplication");
  unsigned long time_Fix8Mult, time_doubleMult;
  TIMED_EXEC( test_multTestcases<Fix8>, time_Fix8Mult, NUM_RUNS );
  TIMED_EXEC( test_multTestcases<double>, time_doubleMult, NUM_RUNS );

  COMMENT("Running testcases for division");
  unsigned long time_Fix8Div, time_doubleDiv;
  TIMED_EXEC( test_divTestcases<Fix8>, time_Fix8Div, NUM_RUNS );
  TIMED_EXEC( test_divTestcases<double>, time_doubleDiv, NUM_RUNS );

  COMMENT("Running testcases for addition");
  unsigned long time_Fix8Add, time_doubleAdd;
  TIMED_EXEC( test_addTestcases<Fix8>, time_Fix8Add, NUM_RUNS );
  TIMED_EXEC( test_addTestcases<double>, time_doubleAdd, NUM_RUNS );

  COMMENT("Running testcases for subtraction");
  unsigned long time_Fix8Sub, time_doubleSub;
  TIMED_EXEC( test_subTestcases<Fix8>, time_Fix8Sub, NUM_RUNS );
  TIMED_EXEC( test_subTestcases<double>, time_doubleSub, NUM_RUNS );

  double incr_Mult = 100.0*double(time_doubleMult)/double(time_Fix8Mult);
  double incr_Div  = 100.0*double(time_doubleDiv)/double(time_Fix8Div);
  double incr_Add  = 100.0*double(time_doubleAdd)/double(time_Fix8Add);
  double incr_Sub  = 100.0*double(time_doubleSub)/double(time_Fix8Sub);

  Serial.println("Op      double\tfixpt\tspeed improvement fixpt over double");
  Serial.print("Mult    "); Serial.print(time_doubleMult); Serial.print("\t"); Serial.print(time_Fix8Mult); Serial.print("\t"); Serial.print(incr_Mult); Serial.println("%");
  Serial.print("Div     "); Serial.print(time_doubleDiv);  Serial.print("\t"); Serial.print(time_Fix8Div);  Serial.print("\t"); Serial.print(incr_Div);  Serial.println("%");
  Serial.print("Add     "); Serial.print(time_doubleAdd);  Serial.print("\t"); Serial.print(time_Fix8Add);  Serial.print("\t"); Serial.print(incr_Add);  Serial.println("%");
  Serial.print("Sub     "); Serial.print(time_doubleSub);  Serial.print("\t"); Serial.print(time_Fix8Sub);  Serial.print("\t"); Serial.print(incr_Sub);  Serial.println("%");

  COMMENT("Test finished");
#endif

#if defined(NO_DOUBLE) && !defined(NO_FIX8)
  test_multTestcases<Fix8>();
  test_divTestcases<Fix8>();
  test_addTestcases<Fix8>();
  test_subTestcases<Fix8>();
#endif

#if !defined(NO_DOUBLE) && defined(NO_FIX8)
  test_multTestcases<double>();
  test_divTestcases<double>();
  test_addTestcases<double>();
  test_subTestcases<double>();
#endif

  while (1) {};
}

void loop()
{
}
