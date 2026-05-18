#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f7xx_hal.h"
#include <stdint.h>

typedef enum {
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_STOP,
    MOTOR_BRAKE
} MotorDirection;

class MotorDriver {
public:
    // Left side:  ENA=PA0/TIM2_CH1, ENB=PA1/TIM2_CH2
    //             IN1/IN2 = motor A direction (PA5, PA6)
    //             IN3/IN4 = motor B direction (PA7, PA8)
    // Right side: ENA=PB6/TIM4_CH1, ENB=PC7/TIM8_CH2
    //             IN5/IN6 = motor C direction (PD14, PD15)
    //             IN7/IN8 = motor D direction (PD0, PD1)
    MotorDriver(
        TIM_HandleTypeDef* htim_LA, uint32_t ch_LA,
        TIM_HandleTypeDef* htim_LB, uint32_t ch_LB,
        GPIO_TypeDef* in1_port, uint16_t in1_pin,
        GPIO_TypeDef* in2_port, uint16_t in2_pin,
        GPIO_TypeDef* in3_port, uint16_t in3_pin,
        GPIO_TypeDef* in4_port, uint16_t in4_pin,
        TIM_HandleTypeDef* htim_RA, uint32_t ch_RA,
        TIM_HandleTypeDef* htim_RB, uint32_t ch_RB,
        GPIO_TypeDef* in5_port, uint16_t in5_pin,
        GPIO_TypeDef* in6_port, uint16_t in6_pin,
        GPIO_TypeDef* in7_port, uint16_t in7_pin,
        GPIO_TypeDef* in8_port, uint16_t in8_pin
    );

    void init();
    void setSpeed(uint8_t speed_percent);
    void drive(MotorDirection dir);
    void stop();
    void brake();

private:
    TIM_HandleTypeDef *_htim_LA, *_htim_LB;
    uint32_t _ch_LA, _ch_LB;
    GPIO_TypeDef *_in1p, *_in2p, *_in3p, *_in4p;
    uint16_t _in1, _in2, _in3, _in4;

    TIM_HandleTypeDef *_htim_RA, *_htim_RB;
    uint32_t _ch_RA, _ch_RB;
    GPIO_TypeDef *_in5p, *_in6p, *_in7p, *_in8p;
    uint16_t _in5, _in6, _in7, _in8;

    uint8_t _speed;

    void _setPWM(TIM_HandleTypeDef* htim, uint32_t ch, uint8_t pct);
    void _setAllPWM(uint8_t pct);
    void _setLeftDir(GPIO_PinState a, GPIO_PinState b,
                     GPIO_PinState c, GPIO_PinState d);
    void _setRightDir(GPIO_PinState a, GPIO_PinState b,
                      GPIO_PinState c, GPIO_PinState d);
};

#endif
