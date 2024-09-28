#pragma once

#include "Arduino.h"
#include "definitions.h"

void initDisplay();
void displayDesiredWeight(float weight);
void displayGrindingInProgress(float currentWeight);
void displayGrindingComplete(float finalWeight);
void displayError(String errorMessage);