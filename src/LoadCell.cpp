#include "LoadCell.h"
#include "StateMachine.h"
#include "Logger.h"

// RunningAverage weightHistory(100);

// void setupScale() {
//   weightHistory.clear();
//   loadcell.begin(PIN_LOADCELL_DA, PIN_LOADCELL_CL);
//   loadcell.set_scale(420.0983);
//   tareScale();
// }

LoadCell::LoadCell() : weight(0.0), isTaring(false), online(false)
{
}

void LoadCell::begin(int doutPin, int sckPin)
{
    scale.begin(doutPin, sckPin);
    scale.set_scale(420.0983);
    scale.tare(); // Reset the scale to 0
    setOnline();
}

void LoadCell::tare()
{
    isTaring = true;
    scale.tare(); // Tare the scale
    isTaring = false;
}

double LoadCell::getWeight()
{
    return 0.0;
    // return scale.get_units();
}

bool LoadCell::calibrate()
{
    Logger &logger = Logger::getInstance();

    if (!isOnline())
    {
        logger.log(Logger::LOG_LEVEL_WARN, "Cannot calibrate; Load cell not online.");
        return false;
    }

    if (isTaring)
    {
        logger.log(Logger::LOG_LEVEL_WARN, "Cannot calibrate; Load cell is already taring.");
        return false;
    }

    tare();
    logger.log(Logger::LOG_LEVEL_INFO, "Load cell tared.");
}

bool LoadCell::isOnline()
{
    return this->online;
}

void LoadCell::setOnline()
{
    this->online = true;
}

void LoadCell::setOffline()
{
    this->online = false;
}