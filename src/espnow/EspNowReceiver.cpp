#include "espnow/EspNowManager.h"

const uint8_t EspNowManager::BROADCAST_ADDRESS[ESP_NOW_ETH_ALEN] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

EspNowManager::EspNowManager(const uint8_t channel) : CHANNEL(channel) {
    uint8_t addr[ESP_NOW_ETH_ALEN] = {0};
    esp_read_mac(addr, ESP_MAC_WIFI_STA);
    memcpy(this->ADDRESS, addr, ESP_NOW_ETH_ALEN);
}

EspNowManager::~EspNowManager(void) {
}

const uint8_t *const EspNowManager::getAddress(void) const {
    return this->ADDRESS;
}

bool EspNowManager::begin(void) {
    if (!WiFi.mode(WIFI_STA)) {
        return false;
    }
    if (!WiFi.disconnect()) {
        return false;
    }
    initEspNow();
    return true;
}

bool EspNowManager::registerPeer(const uint8_t *addr) {
    esp_now_peer_info_t peerInfo;
    initPeer(addr, peerInfo);
    esp_err_t e = esp_now_add_peer(&peerInfo);
    PRINT_ESP_ERR("EspNowManager::registerPeer(): ", e);
    return e == ESP_OK;
}

bool EspNowManager::unregisterPeer(const uint8_t *addr) {
    esp_err_t e = esp_now_del_peer(addr);
    PRINT_ESP_ERR("EspNowManager::unregisterPeer(): ", e);
    return e == ESP_OK;
}

bool EspNowManager::registerCallback(esp_now_recv_cb_t cb) {
    esp_err_t e = esp_now_register_recv_cb(cb);
    PRINT_ESP_ERR("EspNowManager::registerCallback(esp_now_recv_cb_t): ", e);
    return e == ESP_OK;
}

bool EspNowManager::registerCallback(esp_now_send_cb_t cb) {
    esp_err_t e = esp_now_register_send_cb(cb);
    PRINT_ESP_ERR("EspNowManager::registerCallback(esp_now_send_cb_t): ", e);
    return e == ESP_OK;
}

bool EspNowManager::send(const uint8_t *peerAddr, const uint8_t *data,
                         size_t len) {
    esp_err_t e = esp_now_send(peerAddr, data, len);
    PRINT_ESP_ERR("EspNowManager::send(): ", e);
    return e == ESP_OK;
}

void EspNowManager::initEspNow(void) {
    if (esp_now_init() != ESP_OK) {
        ESP.restart();
    }
}

void EspNowManager::initPeer(const uint8_t *addr, esp_now_peer_info_t &peer) {
    memset(&peer, 0, sizeof(peer));
    memcpy(&(peer.peer_addr), addr, ESP_NOW_ETH_ALEN);
    peer.channel = this->CHANNEL;
    peer.encrypt = 0;
}

#ifdef ENABLE_SERIAL_MONITOR
void EspNowManager::printEspErr(esp_err_t e) {
    switch (e) {
        case ESP_OK:
            SERIAL_PRINT("OK");
            break;
        case ESP_ERR_ESPNOW_NOT_INIT:
            SERIAL_PRINT("ESPNOW is not initialized");
            break;
        case ESP_ERR_ESPNOW_ARG:
            SERIAL_PRINT("invalid argument");
            break;
        case ESP_ERR_ESPNOW_NO_MEM:
            SERIAL_PRINT("Out of memory");
            break;
        case ESP_ERR_ESPNOW_FULL:
            SERIAL_PRINT("ESPNOW peer list is full");
            break;
        case ESP_ERR_ESPNOW_NOT_FOUND:
            SERIAL_PRINT("ESPNOW peer is not found");
            break;
        case ESP_ERR_ESPNOW_INTERNAL:
            SERIAL_PRINT("Internal error");
            break;
        case ESP_ERR_ESPNOW_EXIST:
            SERIAL_PRINT("ESPNOW peer has existed");
            break;
        case ESP_ERR_ESPNOW_IF:
            SERIAL_PRINT("Interface error");
            break;
        default:
            SERIAL_PRINTF("Unknown error %d", e);
            break;
    }
}
#endif
