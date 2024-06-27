#include <Arduino.h>
#include "StateMachine.h"
#include "Logger.h"
#include "Display.h"
#include "RotaryEncoder.h"
#include "LoadCell.h"
#include "GrinderMotor.h"

// Rotary Encoder
#define PIN_ENCODER_CLK 17
#define PIN_ENCODER_DT 16
#define PIN_ENCODER_SW 4

// Loadcell
#define PIN_LOADCELL_DA 9
#define PIN_LOADCELL_CL 10

// Grinder Motor
#define RELAY 13

StateMachine &stateMachine = StateMachine::getInstance();
RotaryEncoder &rotaryEncoder = RotaryEncoder::getInstance();
LoadCell &loadCell = LoadCell::getInstance();
GrinderMotor &grinderMotor = GrinderMotor::getInstance();

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(100);
  }

  Logger::getInstance().setLogLevel(Logger::LOG_LEVEL_DEBUG);

  stateMachine.setState(StateMachine::STATE_MAIN);

  // Initialize display
  Display &display = Display::getInstance();
  display.begin();

  // Initialize rotary encoder
  rotaryEncoder.begin(PIN_ENCODER_CLK, PIN_ENCODER_DT, PIN_ENCODER_SW);

  // Initialize load cell
  // loadCell.begin(PIN_LOADCELL_DA, PIN_LOADCELL_CL);

  // Initialize grinder motor
  grinderMotor.begin(RELAY);

  // Create the display task on core 1
  xTaskCreatePinnedToCore(displayTask, "DisplayTask", 4096, NULL, 1, NULL, 1);
}

void loop()
{
  stateMachine.run();

  // Slight delay for stability
  delay(100);
}