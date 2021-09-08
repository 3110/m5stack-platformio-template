#include "common.h"

#ifdef ENABLE_SERIAL_MONITOR

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

void printMacAddress(const uint8_t *macAddr, const bool newline) {
    static char macStr[ETH_ALEN * 3 + 1] = {0};
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4],
             macAddr[5]);
    if (newline) {
        SERIAL_PRINTLN(macStr);
    } else {
        SERIAL_PRINT(macStr);
    }
}
#endif
