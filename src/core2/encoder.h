#pragma once

#include "Arduino.h"

class Encoder
{
public:
    Encoder(int pinA, int pinB);
    void begin();
    int32_t getPosition();
    void setPosition(int32_t position);

    void handleInterruptA();
    void handleInterruptB();

    void disable();
    void enable();

private:
    int _pinA;
    int _pinB;
    bool _enabled;
    volatile int32_t _position;
    volatile bool _lastA;
    volatile bool _lastB;
};