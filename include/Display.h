#pragma once

#include <Arduino.h>
#include "TFT_eSPI.h"
#include "StateMachine.h"
#include "Logger.h"
#include <math.h>

/*
 * Fonts & Colours
 */
#define WEIGHT_FONT_SMALL FONT7RLE
#define WEIGHT_FONT_MEDIUM FONT32RLE
#define WEIGHT_FONT_LARGE FONT64RLE

#define DISPLAY_WIDTH 240
#define DISPLAY_HEIGHT 320

class Display
{
public:
    static Display &getInstance()
    {
        static Display instance;
        return instance;
    }

    void begin();
    void run();

private:
    Display();
    ~Display() = default;

    // Delete copy constructor and assignment operator to prevent copies of the singleton instance
    Display(const Display &) = delete;
    Display &operator=(const Display &) = delete;

    TFT_eSPI tft;
    TFT_eSprite sprite;
    void updateDisplay();
};

void displayTask(void *pvParameters);