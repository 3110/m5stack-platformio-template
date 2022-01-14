#include <M5Stack.h>

#include "Debug.h"

const bool ENABLE_SERIAL = true;

const bool ENABLE_LCD = true;
const bool ENABLE_SD = true;
const bool ENABLE_I2C = false;
#define M5_BEGIN M5.begin(ENABLE_LCD, ENABLE_SD, ENABLE_SERIAL, ENABLE_I2C)

#ifdef ENABLE_BLE_KEYBOARD
#include "BLEKeyboardController.h"
const uint8_t SEND_KEY = ' ';
const char DEVICE_NAME[] = "M5 Keyboard";
const char DEVICE_MANUFACTURER[] = "M5Stack";
#endif

#ifdef ENABLE_WIFI
#include "WiFiController.h"
#endif

#ifdef ENABLE_ESPNOW
#include "EspNowManager.h"
#endif

#ifdef ENABLE_MQTT
#include "MQTTClient.h"
#endif

#ifdef ENABLE_BLE_KEYBOARD
BLEKeyboardController keyboardController(DEVICE_NAME, DEVICE_MANUFACTURER);
bool isKeyPressed = false;

void doConnect(void) {
    SERIAL_PRINTLN("BLE keyboard: Connected");
}

void doUpdate(BLEKeyboardController& controller) {
    if (isKeyPressed) {
        if (M5.BtnA.isReleased()) {
            isKeyPressed = false;
            controller.release(SEND_KEY);
            SERIAL_PRINTF_LN("Key '%c': Released", SEND_KEY);
        }
    } else {
        if (M5.BtnA.isPressed()) {
            isKeyPressed = true;
            controller.press(SEND_KEY);
            SERIAL_PRINTF_LN("Key '%c': Pressed", SEND_KEY);
        }
    }
}

void doDisconnect(void) {
    SERIAL_PRINTLN("BLE Keyboard: Disconnected");
}
#endif

#ifdef ENABLE_WIFI
WiFiController wifiController;

void connectingWiFiCallback(uint8_t retries) {
}
#endif

#ifdef ENABLE_ESPNOW
EspNowManager espNowManager;
const uint8_t MESSAGE[] = "Hello, world!";
const uint32_t MESSAGE_INTERVAL_MS = 5000;

void OnDataSent(const uint8_t* addr, esp_now_send_status_t status) {
    SERIAL_PRINT("OnDataSent(");
    SERIAL_MAC_ADDRESS_PRINT(addr);
    SERIAL_PRINTF_LN("): %s", status == ESP_NOW_SEND_SUCCESS
                                  ? "Delivery Success"
                                  : "Delivery Fail");
}

void OnDataReceived(const uint8_t* addr, const uint8_t* data, int len) {
    SERIAL_PRINT("OnDataReceived(");
    SERIAL_MAC_ADDRESS_PRINT(addr);
    SERIAL_PRINTLN("): [");
    SERIAL_DUMP(data, len);
    SERIAL_PRINTLN("]");
}
#endif

#ifdef ENABLE_MQTT
const char MQTT_BROKER[] = "192.168.11.82";
const char SAMPLE_TOPIC[] = "m5stack/sample";

const size_t MESSAGE_BUFFER_SIZE = 256;
const size_t MAX_CLIENT_ID_PREFIX_LEN = 13;
const char CLIENT_ID_PREFIX[MAX_CLIENT_ID_PREFIX_LEN + 1] = "m5stack-mqtt-";
const size_t MAX_CLIENT_ID_LEN = MAX_CLIENT_ID_PREFIX_LEN + ETH_ALEN * 2;
const unsigned long PUBLISH_INTERVAL_MS = 5000;

char CLIENT_ID[MAX_CLIENT_ID_LEN + 1] = {0};
uint8_t MAC_ADDRESS[ETH_ALEN + 1] = {0};
char msg[MESSAGE_BUFFER_SIZE] = {0};
unsigned long lastPublished = 0;
uint32_t publishCount = 0;

bool setMacAddress(uint8_t* buf, size_t size) {
    if (size < ETH_ALEN) {
        return false;
    }
    esp_read_mac(MAC_ADDRESS, ESP_MAC_WIFI_STA);
    return true;
}

void connectingMQTTCallback(uint8_t retries) {
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    SERIAL_PRINTF("Message arrived [%s]", topic);
    for (int i = 0; i < length; i++) {
        SERIAL_PRINT((char)payload[i]);
    }
    SERIAL_PRINTLN();
}

MQTTClient mqttClient;
#endif

void setup(void) {
    M5_BEGIN;
#if defined(ENABLE_WIFI) && !defined(ENABLE_MQTT)
    wifiController.begin(connectingWiFiCallback);
#endif

#ifdef ENABLE_BLE_KEYBOARD
    keyboardController.begin();
#endif

#ifdef ENABLE_ESPNOW
    espNowManager.begin();
    espNowManager.registerCallback(OnDataSent);
    espNowManager.registerCallback(OnDataReceived);
    espNowManager.registerPeer(EspNowManager::BROADCAST_ADDRESS);
#endif

#ifdef ENABLE_MQTT
    setMacAddress(MAC_ADDRESS, sizeof(MAC_ADDRESS));
    SERIAL_PRINT("Mac Address:");
    SERIAL_MAC_ADDRESS_PRINTLN(MAC_ADDRESS);

    snprintf(CLIENT_ID, sizeof(CLIENT_ID), "%s%02x%02x%02x%02x%02x%02x",
             CLIENT_ID_PREFIX, MAC_ADDRESS[0], MAC_ADDRESS[1], MAC_ADDRESS[2],
             MAC_ADDRESS[3], MAC_ADDRESS[4], MAC_ADDRESS[5]);
    mqttClient.setId(CLIENT_ID);
    if (!mqttClient.begin(connectingMQTTCallback)) {
        while (true) {
        }
    }
    mqttClient.setup(mqttCallback, MQTT_BROKER);
#endif
}

void loop(void) {
    M5.update();
#ifdef ENABLE_WIFI
    wifiController.update();
#endif

#ifdef ENABLE_BLE_KEYBOARD
    keyboardController.update(doConnect, doUpdate, doDisconnect);
#endif

#ifdef ENABLE_ESPNOW
    espNowManager.send(EspNowManager::BROADCAST_ADDRESS, MESSAGE,
                       sizeof(MESSAGE));
    delay(MESSAGE_INTERVAL_MS);
#endif

#ifdef ENABLE_MQTT
    if (!mqttClient.connected()) {
        if (mqttClient.reconnect()) {
            if (mqttClient.subscribe(SAMPLE_TOPIC, 0)) {
                SERIAL_PRINTF_LN("Subscribe: %s", SAMPLE_TOPIC);
            } else {
                SERIAL_PRINTF_LN("Failed to Subscribe: %s", SAMPLE_TOPIC);
            }
        } else {
        }
    }
    mqttClient.update();
    unsigned long now = millis();
    if (now - lastPublished > PUBLISH_INTERVAL_MS) {
        lastPublished = now;
        snprintf(msg, MESSAGE_BUFFER_SIZE, "Hello world #%u from %s.",
                 ++publishCount, CLIENT_ID);
        mqttClient.publish(SAMPLE_TOPIC, msg);
    }
#endif
}