#include "RotaryEncoder.h"
#include "StateMachine.h"
#include "Logger.h"

RotaryEncoder::RotaryEncoder()
    : pinA(0), pinB(0), pinButton(0), desiredCoffeeWeight(DEFAULT_COFFEE_WEIGHT),
      switchPressedFlag(false), CLKPrev(LOW), lastSwitchPressTime(0)
{
}

void RotaryEncoder::begin(int pinA, int pinB, int pinButton)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->pinButton = pinButton;

    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
    pinMode(pinButton, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(pinA), handleRotation, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pinButton), handleSwitchClick, FALLING);
}

// TODO make thread-safe so Display class can use it safely
double RotaryEncoder::getDesiredCoffeeWeight() 
{
    return desiredCoffeeWeight;
}

// Handler for button press; Only sets flag for the next program run. Debounced.
void IRAM_ATTR RotaryEncoder::handleSwitchClick()
{
    long lastSwitchPressTime = RotaryEncoder::getInstance().lastSwitchPressTime;
    if ((millis() - lastSwitchPressTime) > 200 /* Debounce delay */)
    {
        RotaryEncoder::getInstance().switchPressedFlag = true;
        RotaryEncoder::getInstance().lastSwitchPressTime = millis();
    }
}

void RotaryEncoder::handleRotation()
{
    bool DT = digitalRead(RotaryEncoder::getInstance().pinA);
    bool CLK = digitalRead(RotaryEncoder::getInstance().pinB);

    if (RotaryEncoder::getInstance().CLKPrev == LOW && CLK == HIGH)
    {
        if (StateMachine::getInstance().getState() == StateMachine::STATE_MAIN)
        {
            if (DT == HIGH)
            {
                if (RotaryEncoder::getInstance().desiredCoffeeWeight < MAX_COFFEE_WEIGHT)
                    RotaryEncoder::getInstance().desiredCoffeeWeight += COFFEE_WEIGHT_STEP;
            }
            else
            {
                if (RotaryEncoder::getInstance().desiredCoffeeWeight > MIN_COFFEE_WEIGHT)
                    RotaryEncoder::getInstance().desiredCoffeeWeight -= COFFEE_WEIGHT_STEP;
            }
            Logger::getInstance().log(Logger::LOG_LEVEL_DEBUG, ("Desired Coffee Weight: " + String(RotaryEncoder::getInstance().desiredCoffeeWeight)).c_str());
        }
    }
    RotaryEncoder::getInstance().CLKPrev = CLK;
}

bool RotaryEncoder::isSwitchPressed(){
    if(switchPressedFlag){
        switchPressedFlag = false;
        return true;
    }
    else return false;
}

bool RotaryEncoder::isButtonPressed(){
    if(buttonPressedFlag){
        buttonPressedFlag = false;
        return true;
    }
    else return false;
}
