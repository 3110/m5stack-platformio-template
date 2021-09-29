#ifndef COMMON_H_
#define COMMON_H_

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

#ifdef ENABLE_SERIAL_MONITOR
#include <Arduino.h>
#define SERIAL_BEGIN(a) Serial.begin(a)
#define SERIAL_PRINT(a) Serial.print(a)
#define SERIAL_PRINTLN(a) Serial.println(a)
#define SERIAL_PRINTF(...) Serial.printf(__VA_ARGS__)
#define SERIAL_PRINTF_LN(...)       \
    {                               \
        Serial.printf(__VA_ARGS__); \
        Serial.println();           \
    }
#define SERIAL_MAC_ADDRESS_PRINT(a) printMacAddress(a, false)
#define SERIAL_MAC_ADDRESS_PRINTLN(a) printMacAddress(a, true)
#define SERIAL_DUMP(d, l) dump(d, l)
extern void printMacAddress(const uint8_t* macAddr, const bool newline);
extern void dump(const uint8_t* data, size_t len, uint8_t indent = 2,
                 uint8_t width = 16);
#else
#define SERIAL_START
#define SERIAL_BEGIN(a)
#define SERIAL_PRINT(a)
#define SERIAL_PRINTLN(a)
#define SERIAL_PRINTF(...)
#define SERIAL_PRINTF_LN(...)
#define SERIAL_MAC_ADDRESS_PRINT(a)
#define SERIAL_MAC_ADDRESS_PRINTLN(a)
#define SERIAL_DUMP(d, l)
#endif

#endif