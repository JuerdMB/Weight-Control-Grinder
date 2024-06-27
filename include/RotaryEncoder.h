#pragma once

#include <Arduino.h>

#define DEFAULT_COFFEE_WEIGHT 18.0
#define MIN_COFFEE_WEIGHT 1.0
#define MAX_COFFEE_WEIGHT 25.0
#define COFFEE_WEIGHT_STEP 0.1


class RotaryEncoder {
public:
    static RotaryEncoder& getInstance() {
        static RotaryEncoder instance;
        return instance;
    }

    void begin(int pinA, int pinB, int pinButton);
    
    double getDesiredCoffeeWeight();
    bool isSwitchPressed();
    bool isButtonPressed();

private:
    RotaryEncoder();
    ~RotaryEncoder() = default;

    // Delete copy constructor and assignment operator to prevent copies of the singleton instance
    RotaryEncoder(const RotaryEncoder&) = delete;
    RotaryEncoder& operator=(const RotaryEncoder&) = delete;

    static void IRAM_ATTR handleRotation();
    static void IRAM_ATTR handleSwitchClick();

    int pinA, pinB, pinButton;
    double desiredCoffeeWeight;

    volatile bool switchPressedFlag, buttonPressedFlag;
    
    volatile bool CLKPrev;
    unsigned long lastSwitchPressTime;

    void handleButton();
};