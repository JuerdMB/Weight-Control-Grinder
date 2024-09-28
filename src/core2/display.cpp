#include "display.h"
#include "config.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); // Create TFT instance

// TODO work with sprites to avoid flickering

void initDisplay()
{
    tft.init();
    tft.setRotation(1); // Adjust rotation as needed
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM); // Center datum for easier positioning
    tft.setSwapBytes(true);     // For correct color representation in 16-bit color
}

void displayDesiredWeight(float weight)
{
    // Create a sprite
    TFT_eSprite sprite = TFT_eSprite(&tft);
    sprite.createSprite(tft.width(), tft.height());

    // Clear the sprite
    sprite.fillSprite(TFT_BLACK);

    // Calculate positions
    int16_t centerX = tft.width() / 2;
    int16_t centerY = tft.height() / 2;

    // Prepare weight string components
    int intPart = (int)weight;
    int decimalPart = (int)((weight - intPart) * 10);

    // Load fonts
    sprite.loadFont("NotoMonoBold36"); // Monospace font for integer part
    sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    sprite.setTextDatum(MC_DATUM); // Center datum

    // Calculate widths for alignment
    int16_t intWidth = sprite.textWidth(String(intPart));
    int16_t decimalWidth = sprite.textWidth(String(decimalPart));   // TODO: Take into account the font and font sizes of int and decimal part
    int16_t dotWidth = sprite.textWidth(".");

    // Fixed positions for decimal point
    int16_t totalWidth = intWidth + dotWidth + decimalWidth;
    int16_t startX = centerX - (totalWidth / 2);

    // Draw integer part
    sprite.drawString(String(intPart), startX, centerY - 20);

    // Draw decimal point
    startX += intWidth;
    sprite.drawString(".", startX, centerY - 20);

    // Draw decimal part in smaller font
    sprite.loadFont("NotoMonoBold24");
    startX += dotWidth;
    sprite.drawString(String(decimalPart), startX, centerY - 10); // Slightly lower for alignment

    // Draw 'g' symbol
    startX += decimalWidth + 10; // Add spacing
    sprite.drawString("g", startX, centerY - 10);

    // Draw "Press to start" at the bottom center
    sprite.setTextDatum(BC_DATUM);             // Bottom center datum
    sprite.setTextColor(TFT_BLACK, TFT_BLACK); // Black text
    sprite.loadFont("NotoSansBold18");
    sprite.drawString("Press to start", centerX, tft.height() - 10);

    // Push the sprite to the display
    sprite.pushSprite(0, 0);

    // Delete the sprite
    sprite.deleteSprite();
}

void displayGrindingInProgress(float currentWeight) {
    // Create a sprite for the grinding state
    TFT_eSprite grindingInProgressSprite = TFT_eSprite(&tft);

    // Define sprite size and position
    int16_t width = tft.width();
    int16_t height = tft.height();

    grindingInProgressSprite.createSprite(width, height);

    // Clear the sprite
    grindingInProgressSprite.fillSprite(TFT_BLACK);

    // Draw current weight
    grindingInProgressSprite.setTextColor(TFT_WHITE, TFT_BLACK);
    grindingInProgressSprite.setTextDatum(MC_DATUM);
    grindingInProgressSprite.loadFont("NotoSansBold36");
    char weightStr[10];
    snprintf(weightStr, sizeof(weightStr), "%.1f g", currentWeight);
    grindingInProgressSprite.drawString(weightStr, width / 2, height / 2);

    // Push the sprite to the display
    grindingInProgressSprite.pushSprite(0,0);

    // Delete the sprite
    grindingInProgressSprite.deleteSprite();
}

void displayGrindingComplete(float finalWeight)
{
    // Create a sprite for the grinding state
    TFT_eSprite sprite = TFT_eSprite(&tft);
    sprite.createSprite(tft.width(), tft.height());

    // Clear the sprite
    sprite.fillSprite(TFT_BLACK);

    // Calculate positions
    int16_t centerX = tft.width() / 2;
    int16_t centerY = tft.height() / 2;

    // Prepare weight string components
    int intPart = (int)finalWeight;
    int decimalPart = (int)((finalWeight - intPart) * 10);

    // Draw current weight
    sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    sprite.setTextDatum(MC_DATUM);
    sprite.loadFont("NotoSansBold36");
    char weightStr[10];
    snprintf(weightStr, sizeof(weightStr), "%.1f g", finalWeight);
    sprite.drawString(weightStr, centerX, centerY);

    // Push the sprite to the display
    sprite.pushSprite(0, 0);

    // Delete the sprite
    sprite.deleteSprite();



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

void displayError(String errorMessage)
{
    tft.fillScreen(TFT_RED);
    tft.setCursor(0, 40);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE, TFT_RED);
    tft.println("Error:");
    tft.setCursor(0, 80);
    tft.setTextSize(2);
    tft.println(errorMessage);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Reset text color
}