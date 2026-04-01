#include "TFT_lib.h"

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
    tft_lib.setSwapBytes(true);
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

// ==================================================
// Draw Static Progress Bar
// Must Using Millis or Timer for prevent a Flicker
void TFT_Lib::pBar(uint8_t input, uint8_t x_pos, uint8_t y_pos, uint8_t width, uint8_t height, uint16_t color1, uint16_t color2) {
    input = constrain(map(input, 0, 100, 0, width - 2), 0, width - 2);
    
    tft_lib.drawRect(x_pos, y_pos, width, height, color1);
    //tft_lib.fillScreen(TFT_BLACK);
    tft_lib.fillRect(input + x_pos, y_pos + 1, width - input - 1, height - 2, grey);
    tft_lib.fillRect(x_pos + 1, y_pos + 1, input, height - 2, color2);
}

// ===================================================
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
    tft_lib.fillRect((inputLast[no_bar] + 1) + x_pos, y_pos + 1, width - (inputLast[no_bar] + 2), height - 2, grey);    // Refresh Progress Bar
    tft_lib.fillRect(x_pos + 1, y_pos + 1, inputLast[no_bar], height - 2, color2);      // Draw Progress Bar
}

// ===============================================================================
// Draw WiFi Level Icon  Progress
void TFT_Lib::WiFiLevel_Icon(uint8_t x_pos, uint8_t y_pos, uint8_t signal_level) {
    uint16_t grey_icon = tft_lib.color565(94, 94, 94);
    uint16_t black_icon = TFT_BLACK;
    uint16_t white_icon = TFT_WHITE;

    //Icon5 = Strongest Signal [lev:4]
    static const uint16_t wifi_5[] PROGMEM = {
        black_icon, black_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        black_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        black_icon, white_icon, white_icon, white_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        white_icon, white_icon, white_icon, black_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        white_icon, white_icon, black_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, white_icon, white_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, black_icon, white_icon, white_icon, white_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, black_icon, black_icon,
        white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon
    };
    
    //Icon4 = Strong Signal [lev:3]
    static const uint16_t wifi_4[] PROGMEM = {
        black_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, grey_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, grey_icon, black_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, white_icon, white_icon, white_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, black_icon, white_icon, white_icon, black_icon, black_icon, white_icon, white_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon
    };

    //Icon3 = Weak Signal [lev:2]
    static const uint16_t wifi_3[] PROGMEM = {
        black_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, grey_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, grey_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, black_icon, white_icon, white_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, white_icon, white_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, white_icon, white_icon, black_icon, white_icon, white_icon
    };

    //Icon2 = Weakless Signal [lev:1]
    static const uint16_t wifi_2[] PROGMEM = {
        black_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, grey_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, grey_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, white_icon, white_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, white_icon, white_icon
    };

    //Icon0 = Lost Signal [lev:0]
    static const uint16_t wifi_1[] PROGMEM = {
        black_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, grey_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, grey_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, black_icon, black_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, grey_icon
    };

    if(signal_level == 5)
        tft_lib.pushImage(x_pos, y_pos, 11, 11, wifi_5);    // Strongest signal
    else if(signal_level == 4) 
        tft_lib.pushImage(x_pos, y_pos, 11, 11, wifi_4);    // Strong signal
    else if(signal_level == 3) 
        tft_lib.pushImage(x_pos, y_pos, 11, 11, wifi_3);    // Weak Signal
    else if(signal_level == 2) 
        tft_lib.pushImage(x_pos, y_pos, 11, 11, wifi_2);    // Weakless Signal
    else if(signal_level == 1) 
        tft_lib.pushImage(x_pos, y_pos, 11, 11, wifi_1);    // No Signal
}

// ==============================================================================
// Draw MQTT Icon State
void TFT_Lib::MQTTstate_Icon(uint8_t x_pos, uint8_t y_pos, bool connect_status) {
    uint16_t grey_icon = tft_lib.color565(94, 94, 94);
    uint16_t white_icon = TFT_WHITE;
    uint16_t black_icon = TFT_BLACK;

    // MQTT connected icon
    static const uint16_t icon_mqtt1[] PROGMEM = {
        black_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, black_icon,
        white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, black_icon, black_icon, white_icon, black_icon, black_icon, black_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, black_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, black_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, black_icon, black_icon, white_icon, black_icon, white_icon, black_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, black_icon, white_icon, black_icon, white_icon,
        white_icon, white_icon, black_icon, white_icon, white_icon, black_icon, white_icon, black_icon, white_icon, black_icon, white_icon, 
        white_icon, black_icon, black_icon, white_icon, black_icon, black_icon, white_icon, black_icon, white_icon, black_icon, white_icon,
        white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon,
        black_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, white_icon, black_icon
    };

    // MQTT not Connected Icon
    static const uint16_t icon_mqtt2[] PROGMEM = {
        black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, black_icon,
        grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, black_icon, black_icon, grey_icon, black_icon, black_icon, black_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, black_icon, black_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon,
        grey_icon, grey_icon, black_icon, grey_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon, 
        grey_icon, black_icon, black_icon, grey_icon, black_icon, black_icon, grey_icon, black_icon, grey_icon, black_icon, grey_icon,
        grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon,
        black_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, grey_icon, black_icon
    };

    if(!connect_status)
        tft_lib.pushImage(x_pos, y_pos, 11, 11, icon_mqtt2);
    else
        tft_lib.pushImage(x_pos, y_pos, 11, 11, icon_mqtt1);
}