#pragma once

// Pin Definitions
#define MOTOR_PIN          26
#define LOAD_CELL_DOUT_PIN 32
#define LOAD_CELL_SCK_PIN  33
#define ENCODER_PIN_A      34
#define ENCODER_PIN_B      35
#define BUTTON_PIN         25
#define OLED_SDA_PIN       21
#define OLED_SCL_PIN       22

// UI Default Parameters
#define DEFAULT_COFFEE_WEIGHT 18.0f
#define MAX_COFFEE_WEIGHT 24.0f

// Task Delays
#define OPERATIONAL_TASK_DELAY 10    // in milliseconds
#define UI_TASK_DELAY          20    // in milliseconds

// Kalman Filter Parameters
#define KALMAN_Q 0.001
#define KALMAN_R 0.1

// Learning Algorithm Parameters
#define INITIAL_OGO 0.5              // Initial Over-Grind Offset in grams
#define ALPHA       0.2              // Learning rate for the exponential moving average

// SD Card
#define SD_CS_PIN   5