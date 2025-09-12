#ifndef MC38_H
#define MC38_H

#include <Arduino.h>

class MC38 {
private:
    int gpio_pin;          
    unsigned long last_debounce_time;  // For debouncing
    static const unsigned long DEBOUNCE_DELAY = 50; // 50ms debounce
    bool APART = LOW;
    bool TOGETHER = HIGH;

public:
    MC38(int pin) : gpio_pin(pin), last_debounce_time(0) {
        pinMode(gpio_pin, INPUT_PULLUP);
    }
    bool AreMagnetApart() {
        if (digitalRead(gpio_pin) == APART) {
            return true;
        } else {
            return false;
        }
    }

    bool AreMagnetTogether() {
        if (digitalRead(gpio_pin) == TOGETHER) {
            return true;
        } else {
            return false;
        }
    }

    int getRawReading() {
        return digitalRead(gpio_pin);
    }

    int getGPIOPin() {
        return gpio_pin;
    }
};

#endif // MC38_H
