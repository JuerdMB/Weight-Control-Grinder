#include "core1/motor_control.h"
#include "config.h"
#include "Arduino.h"

void initMotorControl(){
    // set motor pin
    pinMode(MOTOR_PIN, OUTPUT);
    stopMotor();
}

void startMotor(){
    digitalWrite(MOTOR_PIN, LOW);
}

void stopMotor(){
    digitalWrite(MOTOR_PIN, HIGH);
}