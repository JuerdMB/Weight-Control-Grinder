#include "display.h"
#include "config.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();  // Create TFT instance

// TODO work with sprites to avoid flickering

void initDisplay() {
    tft.init();
    tft.setRotation(0);  // Adjust rotation as needed
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);

    displayDesiredWeight(10.0f); // Display default weight
}

void displayDesiredWeight(float weight) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 20);
    tft.setTextSize(3);
    tft.println("Set Weight:");
    tft.setCursor(0, 60);
    tft.setTextSize(4);
    tft.print(weight, 1);
    tft.println(" g");
}

void displayGrindingInProgress() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 40);
    tft.setTextSize(4);
    tft.println("Grinding...");
}

void displayGrindingComplete(float finalWeight) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 20);
    tft.setTextSize(3);
    tft.println("Complete!");
    tft.setCursor(0, 60);
    tft.setTextSize(4);
    tft.print("Final: ");
    tft.print(finalWeight, 1);
    tft.println(" g");
}

void displayError(String errorMessage) {
    tft.fillScreen(TFT_RED);
    tft.setCursor(0, 40);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.println("Error:");
    tft.setCursor(0, 80);
    tft.setTextSize(2);
    tft.println(errorMessage);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Reset text color
}