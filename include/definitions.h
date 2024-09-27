#include <Arduino.h>

#pragma once

enum CommandType{ START_GRINDING, CANCEL_GRINDING };
enum StatusType  { GRINDING_COMPLETE, ERROR_OCCURRED };

// Enums for UI and Operational States
enum ui_state       { MAIN, GRINDING_IN_PROGRESS, GRINDING_COMPLETE, ERROR_STATE };
enum OperationalState { IDLE, GRINDING, POST_GRINDING, OP_ERROR };

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
