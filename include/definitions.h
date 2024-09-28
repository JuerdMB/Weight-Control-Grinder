#include <Arduino.h>

#pragma once

enum CommandType{ START_GRINDING, CANCEL_GRINDING };
enum StatusType  { GRINDING_COMPLETE, ERROR_OCCURRED };

// Enums for UI and Operational States
enum ui_state       { UI_MAIN, UI_GRINDING_IN_PROGRESS, UI_GRINDING_COMPLETE, UI_ERROR_STATE };
enum OperationalState { OP_IDLE, OP_GRINDING, OP_POST_GRINDING, OP_ERROR };

// Structures for inter-core communication
struct CommandMessage {
    CommandType type;
    float desiredWeight;
};

struct StatusMessage {
    StatusType type;
    float finalWeight;
    String errorMessage;
};

// Structures for data logging
struct SessionData {
    float desiredWeight;
    float measuredWeightAtStop;
    float finalWeight;
    float overGrindOffset;
};

struct CalibrationData {
    // Add calibration parameters as needed
};