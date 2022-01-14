#pragma once

#include <PubSubClient.h>

#include "WiFiController.h"

class MQTTClient {
public:
    static const int8_t WIFI_RETRY = 30;
    static const uint16_t BROKER_PORT = 1883;
    static const size_t MAX_CLIENT_ID_LEN = 23;
    static const uint16_t RECONNECT_INTERVAL_MS = 5000;

    MQTTClient(void);
    virtual ~MQTTClient(void);

    virtual void setId(const char* id);
    virtual bool begin(void (*connectingCallback)(uint8_t retries));
    virtual bool setup(MQTT_CALLBACK_SIGNATURE, const char* broker,
                       uint16_t port = BROKER_PORT);
    virtual bool update(void);

    virtual bool connected(void);
    virtual bool reconnect(const char* user = nullptr,
                           const char* password = nullptr);
    virtual bool subscribe(const char* topic, uint8_t qos);
    virtual bool publish(const char* topic, const char* payload);

private:
    char clientId[MAX_CLIENT_ID_LEN + 1];
    WiFiController wifiController;
    PubSubClient client;
};
