#pragma once

#include <Arduino.h>

void initInputHandler();
bool isButtonPressed();
int32_t getEncoderPosition();
void resetEncoderPosition();
void disableEncoder();
void enableEncoder();