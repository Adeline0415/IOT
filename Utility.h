#ifndef UTILITY_H
#define UTILITY_H

#include <Arduino.h>

// 定義除錯列印宏
#ifdef DEBUG_MODE
    #define DEBUG_PRINT(x) Serial.println(x)
    #define DEBUG_PRINT_VAR(label, var) do { Serial.print(label); Serial.println(var); } while (0)
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINT_VAR(label, var)
#endif

#endif // UTILITY_H
