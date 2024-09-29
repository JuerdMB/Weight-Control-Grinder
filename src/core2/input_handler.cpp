#include "input_handler.h"
#include "config.h"
#include <Arduino.h>
#include "encoder.h"

Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);

volatile bool button_pressed = false;
volatile long lastButtonPress = 0;

void IRAM_ATTR handleButtonInterrupt() {
    unsigned long now = millis();
    if ((now - lastButtonPress) > 200 /* Debounce delay */)
    {
        button_pressed = true;
        lastButtonPress = now;
    }
}

void initInputHandler()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    encoder.begin();
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, FALLING);
}

bool isButtonPressed()
{
    if (button_pressed)
    {
        button_pressed = false;
        return true;
    }
    return false;
}

int32_t getEncoderPosition() {
    return encoder.getPosition();
}

void resetEncoderPosition() {
    encoder.setPosition(0);
}

void disableEncoder(){
    encoder.disable();
}
void enableEncoder(){
    encoder.enable();
}