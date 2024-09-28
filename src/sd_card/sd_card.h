#pragma once
#include "definitions.h"

void initSDCard();
bool logData(const SessionData &data);
bool loadOverGrindOffset(float &ogo);
bool saveOverGrindOffset(float ogo);