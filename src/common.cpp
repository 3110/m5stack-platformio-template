#include "common.h"

#ifdef ENABLE_SERIAL_MONITOR

#ifndef ETH_ALEN
#define ETH_ALEN 6
#endif

void printMacAddress(const uint8_t* macAddr, const bool newline) {
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

void dump(const uint8_t* data, size_t len, uint8_t indent, uint8_t width) {
    SERIAL_PRINTF_LN("Length: %d", len);
    size_t p = 0;
    for (; p < len; ++p) {
        if (p % width == 0) {
            if (p != 0) {
                SERIAL_PRINT("  |");
                for (size_t cp = p - width; cp < p; ++cp) {
                    SERIAL_PRINTF("%c", isPrintable(data[cp]) ? data[cp] : '.');
                }
                SERIAL_PRINT("|");
                SERIAL_PRINTLN();
            }
            for (size_t i = 0; i < indent; ++i) {
                SERIAL_PRINT(" ");
            }
        }
        SERIAL_PRINTF("%02x ", data[p]);
    }
    if (len % width != 0) {
        uint8_t remain = width - len % width;
        for (size_t cp = 0; cp < remain; ++cp) {
            SERIAL_PRINT("   ");
        }
        SERIAL_PRINT("  |");
        for (size_t cp = len - len % width; cp < len; ++cp) {
            SERIAL_PRINTF("%c", isPrintable(data[cp]) ? data[cp] : '.');
        }
        SERIAL_PRINT("|");
        SERIAL_PRINTLN();
    }
}
#endif
