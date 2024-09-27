#pragma once

#include "inter_core_comm.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

static QueueHandle_t commandQueue;
static QueueHandle_t statusQueue;

void initInterCoreCommunication() {
    commandQueue = xQueueCreate(5, sizeof(CommandMessage));
    statusQueue = xQueueCreate(5, sizeof(StatusMessage));
}

// From Core 2 to Core 1
bool sendCommandToCore1(CommandMessage cmdMsg) {
    return xQueueSend(commandQueue, &cmdMsg, portMAX_DELAY) == pdPASS;
}

bool receiveCommandFromCore2(CommandMessage *cmdMsg) {
    return xQueueReceive(commandQueue, cmdMsg, 0) == pdPASS;
}

// From Core 1 to Core 2
bool sendStatusToCore2(StatusMessage statusMsg) {
    return xQueueSend(statusQueue, &statusMsg, portMAX_DELAY) == pdPASS;
}

bool receiveStatusFromCore1(StatusMessage *statusMsg) {
    return xQueueReceive(statusQueue, statusMsg, 0) == pdPASS;
}