#include "core1/loadCell.h"
#include "config.h"
#include <HX711.h>

HX711 scale;

void initLoadCell() {
    scale.begin(LOAD_CELL_DOUT_PIN, LOAD_CELL_SCK_PIN);
    scale.set_scale();       // Set scale to 1 initially
    scale.tare();            // Reset the scale to zero
}

long readLoadCell() {
    if (scale.is_ready()) {
        return scale.read(); // Read raw data from the load cell
    } else {
        return 0;            // Handle error appropriately
    }
}