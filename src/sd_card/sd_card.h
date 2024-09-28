#pragma once
#include "definitions.h"

void initSDCard();
bool logData(const SessionData &data);
bool loadOverGrindOffset(float &ogo);
bool saveOverGrindOffset(float ogo);

bool saveCalibrationData(long zeroOffset, float scaleFactor);
bool loadCalibrationData(long &zeroOffset, float &scaleFactor);