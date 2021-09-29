#ifndef _ESP_NOW_MANAGER_H
#define _ESP_NOW_MANAGER_H

#include <WiFi.h>
#include <esp_now.h>

#include "common.h"

class EspNowManager {
public:
    static const uint8_t BROADCAST_ADDRESS[ESP_NOW_ETH_ALEN];
    EspNowManager(const uint8_t channel = 0);
    virtual ~EspNowManager(void);

    virtual const uint8_t *const getAddress(void) const;
    virtual bool begin(void);
    virtual bool registerPeer(const uint8_t *addr);
    virtual bool unregisterPeer(const uint8_t *addr);
    virtual bool registerCallback(esp_now_recv_cb_t cb);
    virtual bool registerCallback(esp_now_send_cb_t cb);
    virtual bool send(const uint8_t *peerAddr, const uint8_t *data, size_t len);

protected:
    void initEspNow(void);
    void initPeer(const uint8_t *addr, esp_now_peer_info_t &peer);

#ifdef ENABLE_SERIAL_MONITOR
#define PRINT_ESP_ERR(p, e) \
    SERIAL_PRINT(p);        \
    printEspErr(e);         \
    SERIAL_PRINTLN();

    void printEspErr(esp_err_t e);
#else
#define PRINT_ESP_ERR(p, e)
#endif

private:
    const uint8_t CHANNEL;
    uint8_t ADDRESS[ESP_NOW_ETH_ALEN];
};
#endif