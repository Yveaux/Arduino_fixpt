#include <stdio.h>

#define FLOAT_DECS (8)

#define COMMENT(x) Serial.println(F("\n----" x "----"));
#define STR(x) #x
#define STR2(x) STR(x)
#define TEST(x) \
    if (!(x)) { \
        Serial.println(F("FAILED: " __FILE__ ":" STR2(__LINE__) " " #x) ); \
        status = 1; \
    } else { \
        Serial.println(F("OK: " #x)); \
    }


#define LOG_OVERFLOW_NOT_DETECTED(a, b, op)           \
    Serial.print(double(a), FLOAT_DECS);          \
    Serial.print(F(" " op " "));                 \
    Serial.print(double(b), FLOAT_DECS);          \
    Serial.println(F(" overflow not detected!"));

#define LOG_NEWLINE                               \
    Serial.println(F(""));

#define LOG_EXPRESSION(a, b, res, op)             \
    Serial.print(double(a), FLOAT_DECS);          \
    Serial.print(F(" " op " "));                 \
    Serial.print(double(b), FLOAT_DECS);          \
    Serial.print(F(" = "));                       \
    Serial.println(double(res), FLOAT_DECS);

