#pragma once

#include <Arduino.h>
#include <mutex>

#define MAX_GRINDING_TIME 3000

#define FINISHED_STATE_TIME 500
#define FAILED_STATE_TIME 500

class StateMachine
{
public:
  enum State
  {
    STATE_MAIN,
    STATE_GRINDING_AUTO,
    STATE_GRINDING_MANUAL,
    STATE_GRINDING_FINISHED,
    STATE_GRINDING_FAILED
  };

  static StateMachine &getInstance()
  {
    static StateMachine instance;
    return instance;
  }

  void setState(State newState);
  State getState();
  void run();
  long getCurrentStateTime();

  // Handlers for exceptions in grinding process
  void abortGrindingCycle();

private:
  StateMachine();
  ~StateMachine() = default;

  const char* stateToString(State state);

  // Delete copy constructor and assignment operator to prevent copies of the singleton instance
  StateMachine(const StateMachine &) = delete;
  StateMachine &operator=(const StateMachine &) = delete;

  State currentState;
  std::mutex stateMutex;

  bool startGrindCycle();

  // State handlers
  void handle_state_main();
  void handle_state_grinding_auto();
  void handle_state_grinding_manual();
  void handle_state_grinding_failed();
  void handle_state_grinding_finished();

  unsigned long grindingStartTime, grindingFinishedTime, grindingDuration;
};
