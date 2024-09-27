#pragma once

#include "definitions.h"

void initInterCoreCommunication();
bool sendCommandToCore1(CommandMessage cmdMsg);
bool receiveCommandFromCore2(CommandMessage *cmdMsg);
bool sendStatusToCore2(StatusMessage statusMsg);
bool receiveStatusFromCore1(StatusMessage *statusMsg);