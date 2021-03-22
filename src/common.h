#ifndef COMMON_H_
#define COMMON_H_

#ifdef ENABLE_SERIAL_MONITOR
#include <Arduino.h>
#define SERIAL_BEGIN(a) Serial.begin(a)
#define SERIAL_PRINT(a) Serial.print(a)
#define SERIAL_PRINTLN(a) Serial.println(a)
#define SERIAL_PRINTF(...) Serial.printf(__VA_ARGS__)
#define SERIAL_MAC_ADDERSS_PRINT(a) printMacAddress(a, false)
#define SERIAL_MAC_ADDERSS_PRINTLN(a) printMacAddress(a, true)
extern void printMacAddress(const uint8_t *macAddr, const bool newline);
#else
#define SERIAL_START
#define SERIAL_BEGIN(a)
#define SERIAL_PRINT(a)
#define SERIAL_PRINTLN(a)
#define SERIAL_PRINTF(...)
#define SERIAL_MAC_ADDERSS_PRINT(a)
#define SERIAL_MAC_ADDERSS_PRINTLN(a)
#endif

#endif