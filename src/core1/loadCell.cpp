#include "core1/loadCell.h"
#include "config.h"
#include <HX711.h>
#include "sd_card/sd_card.h"

HX711 scale;

static long zeroOffset = 0;
static float scaleFactor = 1.0f;

void initLoadCell() {
    scale.begin(LOAD_CELL_DOUT_PIN, LOAD_CELL_SCK_PIN);
    tareLoadCell();          // Set zero offset
    // Load calibration data from SD card or set default
    if (!loadCalibrationData(zeroOffset, scaleFactor)) {
        scaleFactor = DEFAULT_SCALE_FACTOR;
    }
}

long readLoadCellRaw() {
    if (scale.is_ready()) {
        return scale.read(); // Read raw data from the load cell
    } else {
        // Handle error appropriately
        return 0;
    }
}

float getLoadCellWeight() {
    long rawValue = readLoadCellRaw();
    float weight = (rawValue - zeroOffset) / scaleFactor;
    return weight;
}

void tareLoadCell() {
    zeroOffset = readLoadCellRaw();
    // Save zeroOffset if needed
}

void calibrateLoadCell(float knownWeight) {
    long rawValue = readLoadCellRaw();
    scaleFactor = (rawValue - zeroOffset) / knownWeight;
    // Save calibration data to SD card
    saveCalibrationData(zeroOffset, scaleFactor);
}