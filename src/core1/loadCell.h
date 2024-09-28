#pragma once

void initLoadCell();
long readLoadCellRaw();                // Returns raw data
float getLoadCellWeight();             // Returns weight in grams
void tareLoadCell();                   // Sets zero offset
void calibrateLoadCell(float knownWeight);  // Calibrates scale factor