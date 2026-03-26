#ifndef TFT_LIBRARIES_H
#define TFT_LIBRARIES_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

extern TFT_eSPI tft_lib;

class TFT_Lib {
    public:
        TFT_Lib();

        void TFTBegin(); // Starting and SetupTFT Display

        void printF(String text_input, uint8_t x_pos, uint8_t y_pos, uint16_t color, uint8_t text_size = 1);    // Print text with Form
        void println(String text_input, uint8_t x_pos, uint8_t y_pos);  // Print text new Form
        void print(String text_input);  // Print Text with previous form

        void pBar(uint8_t input, uint8_t x_pos, uint8_t y_pos, uint8_t width, uint8_t height, uint16_t color1, uint16_t color2 = TFT_CYAN);  // Draw Progress bar Automate
        void soft_pBar(uint8_t input, uint8_t no_bar, uint8_t x_pos, uint8_t y_pos, uint8_t width, uint8_t height, uint16_t color1, uint16_t color2 = TFT_CYAN); // Draw progress bar with soft animation

    private:
        const static uint8_t index_max = 10; // Maximum Soft Bar 
        uint8_t inputLast[index_max] = {0};

};

#endif