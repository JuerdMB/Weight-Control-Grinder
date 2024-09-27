#include "Arduino.h"
#include "config.h"
#include "core1/operational_task.h"
#include "core2/ui_task.h"
#include "communication/inter_core_comm.h"

void setup()
{
    Serial.begin(115200);

    initInterCoreCommunication();

    // Create tasks for Core 1 and Core 2
    xTaskCreatePinnedToCore(
        operationalTask,   // Task function
        "OperationalTask", // Name of task
        8192,              // Stack size
        NULL,              // Parameter
        2,                 // Priority
        NULL,              // Task handle
        0                  // Core ID
    );

    xTaskCreatePinnedToCore(
        uiTask,   // Task function
        "UITask", // Name of task
        8192,     // Stack size
        NULL,     // Parameter
        1,        // Priority
        NULL,     // Task handle
        1         // Core ID
    );
}

void loop()
{
}