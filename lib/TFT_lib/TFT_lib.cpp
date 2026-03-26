#include "TFT_lib.h"

// Global  Variable
unsigned long interval[] = {0, 0};

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

void TFT_Lib::pBar(uint8_t input, uint8_t x_pos, uint8_t y_pos, uint8_t width, uint8_t height, uint16_t color1, uint16_t color2 = TFT_CYAN) {
    unsigned long nowTime = millis();
    int interval = 100;


}

