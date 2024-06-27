#pragma once

#include <Arduino.h>

class GrinderMotor {
public:
    static GrinderMotor& getInstance() {
        static GrinderMotor instance;
        return instance;
    }

    void begin(int relayPin);
    void turnOn();
    void turnOff();

private:
    GrinderMotor();
    ~GrinderMotor() = default;

    // Delete copy constructor and assignment operator to prevent copies of the singleton instance
    GrinderMotor(const GrinderMotor&) = delete;
    GrinderMotor& operator=(const GrinderMotor&) = delete;

    int relayPin;
    bool isOn;

    void handleGrinding();
};