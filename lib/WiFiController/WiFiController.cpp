#include "WiFiController.h"

const char NVS_NAMESPACE[] = "WiFiController";
const char MAX_SSID_LEN = 32;
const char MAX_SSID_PASSWORD_LEN = 64;
const char KEY_SSID[] = "ssid";
const char KEY_SSID_PASSWORD[] = "password";

const char WiFiController::CONFIG_FILE[] = "/settings.json";

WiFiController::WiFiController(void) : nvsLoader(NVS_NAMESPACE), client() {
}

WiFiController::~WiFiController(void) {
}

bool WiFiController::begin(void (*connectingCallback)(uint8_t retries)) {
    char ssid[MAX_SSID_LEN + 1] = {0};
    char passwd[MAX_SSID_PASSWORD_LEN + 1] = {0};

    if (!nvsLoader.begin(CONFIG_FILE)) {
        return false;
    }
    if (!nvsLoader.get(KEY_SSID, ssid, sizeof(ssid))) {
        SERIAL_PRINTLN("SSID is not set");
        return false;
    }
    if (!nvsLoader.get(KEY_SSID_PASSWORD, passwd, sizeof(passwd))) {
        SERIAL_PRINTLN("SSID Password is not set");
        return false;
    }

    return connectWiFi(ssid, passwd, WIFI_RETRY, connectingCallback);
}

bool WiFiController::update(void) {
    return true;
}

bool WiFiController::isConnected(void) const {
    return WiFi.isConnected();
}

bool WiFiController::disconnect(void) {
    return WiFi.disconnect(true);
}

WiFiClient& WiFiController::getClient(void) {
    return this->client;
}

bool WiFiController::connectWiFi(const char* ssid, const char* passwd,
                                 uint8_t retries,
                                 void (*connectingCallback)(uint8_t retries)) {
    SERIAL_PRINTLN();
    SERIAL_PRINT("Connecting to ");
    SERIAL_PRINTLN(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);
    while (WiFi.status() != WL_CONNECTED && --retries >= 0) {
        if (connectingCallback) {
            connectingCallback(retries);
        }
        SERIAL_PRINT(".");
        delay(1000);
    }
    if (retries < 0) {
        return false;
    }
    SERIAL_PRINTLN();
    SERIAL_PRINTF_LN("WiFi connected: %s", ssid);
    SERIAL_PRINTF_LN("IP address: %s", WiFi.localIP().toString().c_str());
    return true;
}
