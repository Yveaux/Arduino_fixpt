#include <stdio.h>
#include <fix16.h>
#include <math.h>

#define NUM_RUNS   (10)

#define COMMENT(x) Serial.println(F("\n----" x "----"));

const int32_t testcases[] = {
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
      if (b == T((int16_t)0)) continue;
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

void test_sqrtTestcasesFix16( void )
{
  unsigned int i;
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    Fix16 a = testcases[i];
    if (a < (int32_t)0) continue;
    f = a.sqrt();
  }
}

void test_sqrtTestcasesDouble( void )
{
  unsigned int i;
  for (i = 0; i < TESTCASES_COUNT; i++)
  {
    double a = testcases[i];
    if (a < 0) continue;
    f = sqrt(a);
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
  Serial.begin(115200);
  while(!Serial.available()){
    Serial.println("Enter any key to begin");
    delay(1000);
  }

  COMMENT("Running testcases for multiplication");
  unsigned long time_Fix16Mult, time_doubleMult;
  TIMED_EXEC( test_multTestcases<Fix16>, time_Fix16Mult, NUM_RUNS );
  TIMED_EXEC( test_multTestcases<double>, time_doubleMult, NUM_RUNS );

  COMMENT("Running testcases for division");
  unsigned long time_Fix16Div, time_doubleDiv;
  TIMED_EXEC( test_divTestcases<Fix16>, time_Fix16Div, NUM_RUNS );
  TIMED_EXEC( test_divTestcases<double>, time_doubleDiv, NUM_RUNS );

  COMMENT("Running testcases for addition");
  unsigned long time_Fix16Add, time_doubleAdd;
  TIMED_EXEC( test_addTestcases<Fix16>, time_Fix16Add, NUM_RUNS );
  TIMED_EXEC( test_addTestcases<double>, time_doubleAdd, NUM_RUNS );

  COMMENT("Running testcases for subtraction");
  unsigned long time_Fix16Sub, time_doubleSub;
  TIMED_EXEC( test_subTestcases<Fix16>, time_Fix16Sub, NUM_RUNS );
  TIMED_EXEC( test_subTestcases<double>, time_doubleSub, NUM_RUNS );

  COMMENT("Running test cases for square root");
  unsigned long time_Fix16Sqrt, time_doubleSqrt;
  TIMED_EXEC( test_sqrtTestcasesFix16, time_Fix16Sqrt, NUM_RUNS );
  TIMED_EXEC( test_sqrtTestcasesDouble, time_doubleSqrt, NUM_RUNS );

  double incr_Mult = 100.0*double(time_doubleMult)/double(time_Fix16Mult);
  double incr_Div  = 100.0*double(time_doubleDiv)/double(time_Fix16Div);
  double incr_Add  = 100.0*double(time_doubleAdd)/double(time_Fix16Add);
  double incr_Sub  = 100.0*double(time_doubleSub)/double(time_Fix16Sub);
  double incr_Sqrt = 100.0*double(time_doubleSqrt)/double(time_Fix16Sqrt);

  Serial.println("Op      double\tfixpt\tspeed improvement fixpt over double");
  Serial.print("Mult    "); Serial.print(time_doubleMult); Serial.print("\t"); Serial.print(time_Fix16Mult); Serial.print("\t"); Serial.print(incr_Mult); Serial.println("%");
  Serial.print("Div     "); Serial.print(time_doubleDiv);  Serial.print("\t"); Serial.print(time_Fix16Div);  Serial.print("\t"); Serial.print(incr_Div);  Serial.println("%");
  Serial.print("Add     "); Serial.print(time_doubleAdd);  Serial.print("\t"); Serial.print(time_Fix16Add);  Serial.print("\t"); Serial.print(incr_Add);  Serial.println("%");
  Serial.print("Sub     "); Serial.print(time_doubleSub);  Serial.print("\t"); Serial.print(time_Fix16Sub);  Serial.print("\t"); Serial.print(incr_Sub);  Serial.println("%");
  Serial.print("Sqrt    "); Serial.print(time_doubleSqrt); Serial.print("\t"); Serial.print(time_Fix16Sqrt); Serial.print("\t"); Serial.print(incr_Sqrt); Serial.println("%");

  COMMENT("Test finished");

  while (1) {};
}

void loop()
{
}
