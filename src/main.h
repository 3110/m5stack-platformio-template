#ifndef MAIN_H_
#define MAIN_H_

#if defined(TARGET_M5STACK)
#include <M5Stack.h>
#elif defined(TARGET_M5STICK)
#include <M5StickC.h>
#elif defined(TARGET_M5ATOM)
#include <M5Atom.h>
#elif defined(TARGET_M5PAPER)
#include <M5EPD.h>
#endif

#include "common.h"

#endif