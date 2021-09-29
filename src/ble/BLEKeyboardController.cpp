#include "ble/BLEKeyboardController.h"

BLEKeyboardController::BLEKeyboardController(const char* name,
                                             const char* manufacturer)
    : bleKeyboard(name, manufacturer), isBleConnected(false) {
}

BLEKeyboardController::~BLEKeyboardController(void) {
    this->bleKeyboard.end();
}

void BLEKeyboardController::begin(void) {
    this->bleKeyboard.begin();
}

void BLEKeyboardController::end(void) {
    this->bleKeyboard.end();
}

bool BLEKeyboardController::update(
    void (*connectCallback)(void),
    void (*updateCallback)(BLEKeyboardController& controller),
    void (*disconnectCallback)(void)) {
    if (this->bleKeyboard.isConnected()) {
        if (!this->isBleConnected) {
            this->isBleConnected = true;
            if (connectCallback) {
                connectCallback();
            }
        }
        if (updateCallback) {
            updateCallback(*this);
        }
    } else {
        if (this->isBleConnected) {
            this->isBleConnected = false;
            if (disconnectCallback) {
                disconnectCallback();
            }
        }
    }
    return true;
}

size_t BLEKeyboardController::press(uint8_t k) {
    return this->bleKeyboard.press(k);
}

size_t BLEKeyboardController::press(const MediaKeyReport k) {
    return this->bleKeyboard.press(k);
}

size_t BLEKeyboardController::release(uint8_t k) {
    return this->bleKeyboard.release(k);
}

size_t BLEKeyboardController::release(const MediaKeyReport k) {
    return this->bleKeyboard.release(k);
}

size_t BLEKeyboardController::write(uint8_t c) {
    return this->bleKeyboard.write(c);
}

size_t BLEKeyboardController::write(const MediaKeyReport c) {
    return this->bleKeyboard.write(c);
}

void BLEKeyboardController::releaseAll(void) {
    this->bleKeyboard.releaseAll();
}
