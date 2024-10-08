# Weight Control Grinder

## Overview

The **Weight Control Grinder** is an embedded system designed to control a grinder (such as a coffee grinder) based on precise weight measurements. It utilizes a load cell to measure the weight of the ground material and automatically stops the grinder when the desired weight is reached. The system incorporates a Kalman filter for accurate weight readings and a learning algorithm to adjust for over-grind offsets. The user interface is displayed on a TFT screen, and user inputs are handled via a rotary encoder and a push button.

## Features

- **Precise Weight Control**: Uses a load cell and Kalman filtering for accurate weight measurements.
- **Learning Algorithm**: Adjusts for over-grind offsets over time to improve accuracy.
- **User Interface**: TFT display showing desired weight, current weight, and status messages.
- **User Inputs**: Rotary encoder for adjusting desired weight and a push button to start or cancel grinding.
- **Motor Control**: Starts and stops the grinder motor based on weight measurements.
- **Data Logging**: Logs session data and calibration parameters to an SD card.
- **Inter-Core Communication**: Utilizes ESP32's dual-core capabilities to separate UI and operational tasks.

## Hardware Requirements

- **Microcontroller**: ESP32
- **Load Cell**: For weight measurements
- **HX711 Load Cell Amplifier**: Interface between the load cell and ESP32
- **TFT Display**: ST7789 240x320 TFT screen
- **Rotary Encoder**: For user input
- **Push Button**: For starting/canceling the grinding process
- **Motor Driver and Grinder Motor**: To control the grinder
- **SD Card Module**: For data logging
- **Additional Components**:
  - Wires and connectors
  - Resistors (as needed)
  - Power supply appropriate for the motor and ESP32

## Software Requirements

- **Development Environment**:
  - [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
- **Arduino Libraries**:
  - [`ArduinoJson`](https://arduinojson.org/)
  - [`HX711`](https://github.com/bogde/HX711)
  - [`TFT_eSPI`](https://github.com/Bodmer/TFT_eSPI)
  - [`SPI`](https://www.arduino.cc/en/reference/SPI)
  - [`SD`](https://www.arduino.cc/en/Reference/SD)
  - **FreeRTOS**: Included with the ESP32 Arduino core for multitasking
- **ESP32 Board Support Package**: Install via the Arduino IDE Boards Manager or PlatformIO