#include "main.h"

const bool ENABLE_SERIAL = true;

#if defined(TARGET_M5STACK)
const bool ENABLE_LCD = true;
const bool ENABLE_SD = true;
const bool ENABLE_I2C = false;
#define M5_BEGIN M5.begin(ENABLE_LCD, ENABLE_SD, ENABLE_SERIAL, ENABLE_I2C)
#elif defined(TARGET_M5STICKC)
const bool ENABLE_LCD = true;
const bool ENABLE_POWER = true;
#define M5_BEGIN M5.begin(ENABLE_LCD, ENABLE_POWER, ENABLE_SERIAL)
#elif defined(TARGET_M5ATOM)
const bool ENABLE_I2C = false;
const bool ENABLE_DISPLAY = true;
#define M5_BEGIN M5.begin(ENABLE_SERIAL, ENABLE_I2C, ENABLE_DISPLAY)
#elif defined(TARGET_M5PAPER)
const bool ENABLE_TOUCH = true;
const bool ENABLE_SD = true;
const bool ENABLE_BATTERY_ADC = true;
const bool ENABLE_I2C = false;
#define M5_BEGIN                                                         \
    M5.begin(ENABLE_TOUCH, ENABLE_SD, ENABLE_SERIAL, ENABLE_BATTERY_ADC, \
             ENABLE_I2C)
#endif

void setup(void) {
    M5_BEGIN;
}

void loop(void) {
    M5.update();
}