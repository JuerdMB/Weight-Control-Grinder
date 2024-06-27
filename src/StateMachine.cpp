#include "StateMachine.h"
#include "Arduino.h"
#include "LoadCell.h"
#include "GrinderMotor.h"
#include "Display.h"
#include "RotaryEncoder.h"
#include <Logger.h>

StateMachine::StateMachine() : currentState(STATE_MAIN)
{
}

void StateMachine::setState(State newState)
{
    Logger &logger = Logger::getInstance();
    // Change the state with thread-safety and log
    std::lock_guard<std::mutex> guard(stateMutex);
    logger.log(Logger::LOG_LEVEL_INFO,
               ("State transition: " + String(stateToString(currentState)) + " -> " + String(stateToString(newState))).c_str());
    currentState = newState;
}

StateMachine::State StateMachine::getState()
{
    std::lock_guard<std::mutex> guard(stateMutex);
    return currentState;
}

const char *StateMachine::stateToString(State state)
{
    switch (state)
    {
    case STATE_MAIN:
        return "STATE_MAIN";
    case STATE_GRINDING_AUTO:
        return "STATE_GRINDING_AUTO";
    case STATE_GRINDING_MANUAL:
        return "STATE_GRINDING_MANUAL";
    case STATE_GRINDING_FINISHED:
        return "STATE_GRINDING_FINISHED";
    case STATE_GRINDING_FAILED:
        return "STATE_GRINDING_FAILED";
    default:
        return "UNKNOWN_STATE";
    }
}

long StateMachine::getCurrentStateTime()
{
    return millis() - grindingStartTime;
}

void StateMachine::run()
{
    switch (getState()) // Do not lock state mutex here or it will cause a deadlock
    {
    case STATE_MAIN:
        handle_state_main();
        break;
    case STATE_GRINDING_AUTO:
        handle_state_grinding_auto();
        break;
    case STATE_GRINDING_MANUAL:
        handle_state_grinding_manual();
        break;
    case STATE_GRINDING_FINISHED:
        handle_state_grinding_finished();
        break;
    case STATE_GRINDING_FAILED:
        handle_state_grinding_failed();
        break;
    default:
        Logger::getInstance().log(Logger::LOG_LEVEL_ERROR, "Unknown state");
        break;
    }
}

void StateMachine::handle_state_main()
{
    Logger &logger = Logger::getInstance();
    logger.log(Logger::LOG_LEVEL_DEBUG, "Executing MAIN state functionality.");

    // See if button was pressed
    if (RotaryEncoder::getInstance().isSwitchPressed())
    {
        logger.log(Logger::LOG_LEVEL_DEBUG, "Switch Clicked.");

        if (!startGrindCycle())
            return;

        // Move to Grinding state
        setState(STATE_GRINDING_AUTO);
    }

    if (RotaryEncoder::getInstance().isButtonPressed())
    {
        logger.log(Logger::LOG_LEVEL_DEBUG, "Button Clicked");

        if (!startGrindCycle())
            return;

        // Move to Grinding state
        setState(STATE_GRINDING_MANUAL);
    }
}

void StateMachine::handle_state_grinding_auto()
{
    Logger::getInstance().log(Logger::LOG_LEVEL_DEBUG, "Executing GRINDING_AUTO state functionality.");

    // If we shoot past the max time
    if (millis() - grindingStartTime > MAX_GRINDING_TIME)
    {
        // Determine time it took
        grindingFinishedTime = millis();
        grindingDuration = grindingFinishedTime - grindingStartTime;
        // Turn motor off
        GrinderMotor &grinderMotor = GrinderMotor::getInstance();
        grinderMotor.turnOff();
        // Log and early break
        Logger::getInstance().log(Logger::LOG_LEVEL_INFO, ("Grinding overtime at " + String(grindingDuration / 1000) + " seconds.").c_str());
        setState(STATE_GRINDING_FAILED);
        return;
    }

    // Check loadcell
    LoadCell &loadCell = LoadCell::getInstance();
    if (!loadCell.isOnline())
    {
        Logger::getInstance().log(Logger::LOG_LEVEL_ERROR, "Loadcell error in grinding state. Returning to main");
        setState(STATE_GRINDING_FAILED);
        return;
    }

    // Measure current weight
    double currentWeight = loadCell.getWeight(); // TODO add kalman filter here

    // Check if we have passed the weight we want
    RotaryEncoder &encoder = RotaryEncoder::getInstance();
    double desiredWeight = encoder.getDesiredCoffeeWeight();
    if (currentWeight >= desiredWeight)
    {
        // Turn the motor OFF
        GrinderMotor &grinderMotor = GrinderMotor::getInstance();
        grinderMotor.turnOff();

        // Determine time it took
        grindingFinishedTime = millis();
        grindingDuration = grindingFinishedTime - grindingStartTime;

        setState(STATE_GRINDING_FINISHED);

        Logger::getInstance().log(Logger::LOG_LEVEL_INFO,
                                  ("Grinding finished after " + String(grindingDuration / 1000) + " seconds with a total of " + String(currentWeight) + " grams.").c_str());
    }
}

void StateMachine::handle_state_grinding_manual()
{
    Logger::getInstance().log(Logger::LOG_LEVEL_DEBUG, "Executing GRINDING_MANUAL state functionality.");
}

void StateMachine::handle_state_grinding_finished()
{
    Logger::getInstance().log(Logger::LOG_LEVEL_DEBUG, "Executing GRINDING_FINISHED state functionality.");
    // if (millis() - grindingFinishedTime > FINISHED_STATE_TIME)
    //   {
    //   }
    setState(STATE_MAIN);
}

void StateMachine::handle_state_grinding_failed()
{
    Logger::getInstance().log(Logger::LOG_LEVEL_DEBUG, "Executing GRINDING_FAILED state functionality.");
    //   if (millis() - grindingFinishedTime > FAILED_STATE_TIME)
    //   {
    //   }
    setState(STATE_MAIN);
}

bool StateMachine::startGrindCycle()
{
    Logger &logger = Logger::getInstance();

    // Set loadcell current weight to 0
    LoadCell &loadCell = LoadCell::getInstance();
    if (!loadCell.calibrate())
    {
        logger.log(Logger::LOG_LEVEL_ERROR, "Cannot calibrate, loadcell is not live. Returning to main.");
        return false;
    }

    // Turn the motor ON
    GrinderMotor &grinderMotor = GrinderMotor::getInstance();
    grinderMotor.turnOn();

    // Set current time as start of grinding cycle
    grindingStartTime = millis();
    logger.log(Logger::LOG_LEVEL_DEBUG, ("Starting grinding at " + String(grindingStartTime)).c_str());

    return true;
}