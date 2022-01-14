#include "MQTTClient.h"

MQTTClient::MQTTClient()
    : clientId{0}, wifiController(), client(wifiController.getClient()) {
}

MQTTClient::~MQTTClient(void) {
}

void MQTTClient::setId(const char* id) {
    snprintf(this->clientId, sizeof(this->clientId), "%s", id);
}

bool MQTTClient::begin(void (*connectingCallback)(uint8_t retries)) {
    return this->wifiController.begin(connectingCallback);
}

bool MQTTClient::setup(MQTT_CALLBACK_SIGNATURE, const char* broker,
                       uint16_t port) {
    this->client.setServer(broker, port).setCallback(callback);
    return true;
}

bool MQTTClient::update(void) {
    bool result = this->wifiController.update();
    result &= this->client.loop();
    return result;
}

bool MQTTClient::connected(void) {
    return this->client.connected();
}

bool MQTTClient::reconnect(const char* user, const char* password) {
    while (!this->client.connected()) {
        SERIAL_PRINTF("Attempting MQTT connection(%s", this->clientId);
        if (user != nullptr && password != nullptr) {
            SERIAL_PRINTF(", user = %s, password = %s", user, password);
        }
        SERIAL_PRINT(") ... ");
        if (this->client.connect(this->clientId, user, password)) {
            SERIAL_PRINTLN("Connected");
        } else {
            SERIAL_PRINTF_LN("Failed: rc = %d try again in %d seconds",
                             this->client.state(),
                             RECONNECT_INTERVAL_MS / 1000);
            delay(RECONNECT_INTERVAL_MS);
        }
    }
    return true;
}

bool MQTTClient::subscribe(const char* topic, uint8_t qos) {
    return this->client.subscribe(topic, qos);
}

bool MQTTClient::publish(const char* topic, const char* payload) {
    return this->client.publish(topic, payload);
}
