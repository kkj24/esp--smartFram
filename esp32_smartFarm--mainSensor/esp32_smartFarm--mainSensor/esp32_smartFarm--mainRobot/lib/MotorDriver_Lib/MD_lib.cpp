#include "MD_Lib.h"

MotorDriv::MotorDriv() {
}

void MotorDriv::begin() {
    // Setup Shift Register Pins
    pinMode(PIN_DATA, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    pinMode(PIN_LATCH, OUTPUT);

    // FIX: Inisialisasi shift register ke kondisi 0x00 (semua motor OFF)
    // Tanpa ini, shift register bisa memiliki nilai random saat startup!
    digitalWrite(PIN_LATCH, LOW);
    delayMicroseconds(5);
    shiftOut(PIN_DATA, PIN_CLK, MSBFIRST, 0x00);
    delayMicroseconds(5);
    digitalWrite(PIN_LATCH, HIGH);

    // Setup PWM Channels
    // Channel 0 & 1 = Motor A (PWM_A1, PWM_A2) — keduanya mendapat pwm1
    // Channel 2 & 3 = Motor B (PWM_B1, PWM_B2) — keduanya mendapat pwm2
    ledcSetup(0, PWM_FREQ, PWM_RES);
    ledcSetup(1, PWM_FREQ, PWM_RES);
    ledcSetup(2, PWM_FREQ, PWM_RES);
    ledcSetup(3, PWM_FREQ, PWM_RES);

    // Attach PWM Pins to Channels
    ledcAttachPin(PIN_PWM_A1, 0);
    ledcAttachPin(PIN_PWM_A2, 1);
    ledcAttachPin(PIN_PWM_B1, 2);
    ledcAttachPin(PIN_PWM_B2, 3);

    // Pastikan semua PWM mulai dari 0 (motor diam)
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    ledcWrite(3, 0);
}

void MotorDriv::run(uint8_t form, uint8_t pwm1, uint8_t pwm2) {
    // Send data to shift register with a slight delay for 74HC595 stability
    digitalWrite(PIN_LATCH, LOW);
    delayMicroseconds(5);
    shiftOut(PIN_DATA, PIN_CLK, MSBFIRST, form);
    delayMicroseconds(5);
    digitalWrite(PIN_LATCH, HIGH);

    // Set Motor Speeds
    ledcWrite(0, pwm1);
    ledcWrite(1, pwm1);
    ledcWrite(2, pwm2);
    ledcWrite(3, pwm2);
}