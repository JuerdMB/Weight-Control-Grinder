#pragma once

#include <Arduino.h>
#include "HX711.h"
#include <RunningAverage.h>

class LoadCell
{
public:
    static LoadCell &getInstance()
    {
        static LoadCell instance;
        return instance;
    }

    void begin(int doutPin, int sckPin);
    double getWeight();
    bool calibrate();

    bool isOnline();
    void setOffline();
    void setOnline();

private:
    LoadCell();
    ~LoadCell() = default;

    // Delete copy constructor and assignment operator to prevent copies of the singleton instance
    LoadCell(const LoadCell &) = delete;
    LoadCell &operator=(const LoadCell &) = delete;

    void tare();

    HX711 scale;
    double weight;
    bool isTaring;

    bool online;
};