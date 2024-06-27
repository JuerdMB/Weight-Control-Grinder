#include "GrinderMotor.h"
#include "StateMachine.h"
#include "Logger.h"

GrinderMotor::GrinderMotor() : relayPin(0), isOn(false) {
}

void GrinderMotor::begin(int relayPin) {
    this->relayPin = relayPin;
    pinMode(relayPin, OUTPUT);
    turnOff();
}

void GrinderMotor::turnOn() {
    if (!isOn) {
        digitalWrite(relayPin, LOW);
        isOn = true;
        Logger::getInstance().log(Logger::LOG_LEVEL_INFO, "Grinder motor turned on.");
    }
}

void GrinderMotor::turnOff() {
    if (isOn) {
        digitalWrite(relayPin, HIGH);
        isOn = false;
        Logger::getInstance().log(Logger::LOG_LEVEL_INFO, "Grinder motor turned off.");
    }
}