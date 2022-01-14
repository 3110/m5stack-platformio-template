#ifndef BLE_KEYBOARD_CONTROLLER_H_
#define BLE_KEYBOARD_CONTROLLER_H_

#include <BleKeyboard.h>

class BLEKeyboardController {
public:
    BLEKeyboardController(const char* name, const char* manufacturer);
    virtual ~BLEKeyboardController(void);

    virtual void begin(void);
    virtual void end(void);

    virtual bool update(
        void (*connectCallback)(void),
        void (*updateCallback)(BLEKeyboardController& controller),
        void (*disconnectCallback)(void));

    virtual size_t press(uint8_t k);
    virtual size_t press(const MediaKeyReport k);
    virtual size_t release(uint8_t k);
    virtual size_t release(const MediaKeyReport k);
    virtual size_t write(uint8_t c);
    virtual size_t write(const MediaKeyReport c);
    virtual void releaseAll(void);

protected:
    inline BleKeyboard& getKeyboard(void) {
        return this->bleKeyboard;
    }

    inline bool isConnected(void) {
        return this->isBleConnected;
    }

private:
    BleKeyboard bleKeyboard;
    bool isBleConnected;
};

#endif