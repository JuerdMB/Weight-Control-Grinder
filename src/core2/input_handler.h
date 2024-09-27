#pragma once

#include <Arduino.h>

volatile long lastButtonPress = 0;

void initInputHandler();
bool isButtonPressed();
int32_t getEncoderPosition();
void resetEncoderPosition();
void disableEncoder();
void enableEncoder();