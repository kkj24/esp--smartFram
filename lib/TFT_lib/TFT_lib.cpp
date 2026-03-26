#include "TFT_lib.h"

// Global Variable
uint16_t grey = tft_lib.color565(64, 64, 64);

TFT_eSPI tft_lib;

TFT_Lib::TFT_Lib() {
    // No Func in Here
};

// ==========================================================
// Begin and Starting TFT Display Automate
void TFT_Lib::TFTBegin() {
    tft_lib.begin();
    tft_lib.init();
    tft_lib.setRotation(2); // Potrait
    tft_lib.fillScreen(TFT_BLACK); // Fill Screen with Black Color

    ledcSetup(1, 10000, 8);
    ledcAttachPin(25, 1);

    ledcWrite(1, 127);  // Backlight Level Brightness
}

// =======================================================================================================
// Print text in TFT Display with Format
void TFT_Lib::printF(String text_input, uint8_t x_pos, uint8_t y_pos, uint16_t color, uint8_t text_size) {
    tft_lib.setCursor(x_pos, y_pos);
    tft_lib.setTextColor(color);
    tft_lib.setTextSize(text_size);
    tft_lib.print(text_input);
}

// =====================================================================
// Print Text in new Position with Previous Format
void TFT_Lib::println(String text_input, uint8_t x_pos, uint8_t y_pos) {
    tft_lib.setCursor(x_pos, y_pos);
    tft_lib.print(text_input);
} 

// =========================================
// Print Text with Previous Format
void TFT_Lib::print(String text_input) {
    tft_lib.print(text_input);
}

// Draw Static Progress Bar
// Must Using Millis or Timer for prevent a Flicker
void TFT_Lib::pBar(uint8_t input, uint8_t x_pos, uint8_t y_pos, uint8_t width, uint8_t height, uint16_t color1, uint16_t color2) {
    input = constrain(map(input, 0, 100, 0, width - 2), 0, width - 2);
    
    tft_lib.drawRect(x_pos, y_pos, width, height, color1);
    //tft_lib.fillScreen(TFT_BLACK);
    tft_lib.fillRect(input + x_pos, y_pos + 1, width - input - 1, height - 2, grey);
    tft_lib.fillRect(x_pos + 1, y_pos + 1, input, height - 2, color2);
}

// Draw Animate Progress Bar
// Must using Millis/Micros or Timmer for Preventt  Flicker and Smoothy Animation
void TFT_Lib::soft_pBar(uint8_t input, uint8_t no_bar, uint8_t x_pos, uint8_t y_pos, uint8_t width, uint8_t height, uint16_t color1, uint16_t color2) {
    input = constrain(map(input, 0, 100, 0, width - 2), 0, width - 2);

    if(inputLast[no_bar] == input)
        inputLast[no_bar] = input;
    else if(inputLast[no_bar] <= input)
        inputLast[no_bar]++;
    else if(inputLast[no_bar] >= input)
        inputLast[no_bar]--;

    tft_lib.drawRect(x_pos, y_pos, width, height, color1);  // Draw Progress ar Outline
    //tft_lib.fillScreen(TFT_BLACK);
    tft_lib.fillRect(inputLast[no_bar] + x_pos, y_pos + 1, width - inputLast[no_bar] - 1, height - 2, grey);    // Refresh Progress Bar
    tft_lib.fillRect(x_pos + 1, y_pos + 1, inputLast[no_bar], height - 2, color2);      // Draw Progress Bar
}