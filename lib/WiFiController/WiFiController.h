#ifndef WIFI_CONTROLLER_H_
#define WIFI_CONTROLLER_H_

#include <WiFi.h>

#include "NVSLoader.h"

class WiFiController {
public:
    static const int8_t WIFI_RETRY = 30;
    static const char CONFIG_FILE[];

    WiFiController(void);
    virtual ~WiFiController(void);

    virtual bool begin(void (*connectingCallback)(uint8_t retries));
    virtual bool update(void);
    virtual bool isConnected(void) const;
    virtual bool disconnect(void);

    virtual WiFiClient& getClient(void);

protected:
    virtual bool connectWiFi(const char* ssid, const char* passwd,
                             uint8_t retries,
                             void (*connectingCallback)(uint8_t retries));

private:
    NVSLoader nvsLoader;
    WiFiClient client;
};

#endif
