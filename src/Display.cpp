#include "Display.h"
#include "StateMachine.h"
#include "RotaryEncoder.h"
#include "Free_Fonts.h"

Display::Display() : tft(TFT_eSPI()), sprite(TFT_eSprite(&tft))
{
}

void Display::begin()
{
  tft.init();
  tft.setRotation(0);
  sprite.setColorDepth(8);
}

void Display::run()
{
  while (true)
  {
    updateDisplay();
    vTaskDelay(pdMS_TO_TICKS(10)); // Update display every 10 ms
  }
}

void Display::updateDisplay()
{
  StateMachine &stateMachine = StateMachine::getInstance();
  StateMachine::State currentState = stateMachine.getState();

  // Create the sprite with dimensions matching the display
  sprite.createSprite(tft.width(), tft.height());
  sprite.fillSprite(TFT_BLACK);

  switch (currentState)
  {
  case StateMachine::STATE_MAIN:

    sprite.setTextSize(1);
    sprite.setTextDatum(TL_DATUM);
    sprite.setFreeFont(NULL);
    sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    sprite.drawString("Main State", 10, 10, 2);

    double desired;
    desired = RotaryEncoder::getInstance().getDesiredCoffeeWeight();
    // fractional = desired - (int)desired;
    // integer = desired - fractional;

    // Draw grams before comma
    sprite.setTextSize(5);
    sprite.setFreeFont(FMB24);
    sprite.setTextDatum(TC_DATUM);
    sprite.drawFloat(desired, 1, tft.width() / 2 - 2, tft.height() / 2 - 10, 2);

    // Push the sprite to the display
    sprite.pushSprite(0, 0, TFT_TRANSPARENT);
    break;

  case StateMachine::STATE_GRINDING_AUTO:
    sprite.setTextSize(1);
    sprite.setTextDatum(TL_DATUM);
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.drawString("Auto grinding", 10, 10, 2);

    // Draw tijd die hij al bezig is
    sprite.drawFloat((float)(stateMachine.getCurrentStateTime() / 100) / 10., 1, 100, 20);

    // Draw huidig gewicht
    sprite.setTextSize(5);
    sprite.setFreeFont(FMB24);
    sprite.drawFloat(0.0, 1, tft.width() / 2 - 2, tft.height() / 2 - 10);
    break;

   case StateMachine::STATE_GRINDING_MANUAL:
    tft.drawString("Manual grinding.", 10, 10, 2);
    break;

  case StateMachine::STATE_GRINDING_FINISHED:
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Grinding Finished", 10, 10, 2);
    break;

  case StateMachine::STATE_GRINDING_FAILED:
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("Grinding Failed", 10, 10, 2);
    break;

  default:
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Unknown State", 10, 10, 2);
    break;

  }
}

void displayTask(void *pvParameters)
{
  Display &display = Display::getInstance();
  display.run();
  vTaskDelay(pdMS_TO_TICKS(10));
}