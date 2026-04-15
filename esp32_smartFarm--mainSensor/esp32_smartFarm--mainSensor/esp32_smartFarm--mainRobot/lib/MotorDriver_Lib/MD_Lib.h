#ifndef __MOTOR_DRIVER_LIBRARIES_H__
#define __MOTOR_DRIVER_LIBRARIES_H__

#include <Arduino.h>

// Shift Register Pins
#define PIN_DATA  19
#define PIN_CLK   21
#define PIN_LATCH 18

// PWM Pins for Motors
#define PIN_PWM_A1 23
#define PIN_PWM_A2 22
#define PIN_PWM_B1 26
#define PIN_PWM_B2 27

// PWM Configuration
#define PWM_FREQ 10000
#define PWM_RES  8

class MotorDriv {
public:
    MotorDriv();

    void begin();
    void run(uint8_t form, uint8_t pwm1, uint8_t pwm2);
};

#endif