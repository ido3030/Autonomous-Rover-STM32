#pragma once

#include "stm32f7xx_hal.h"

class MotorDriver {
public:
    enum class Direction { FORWARD, BACKWARD, STOP, BRAKE };

    MotorDriver(TIM_HandleTypeDef* htim_left,  uint32_t ch_left,
                TIM_HandleTypeDef* htim_right, uint32_t ch_right,
                GPIO_TypeDef* in1_port, uint16_t in1_pin,
                GPIO_TypeDef* in2_port, uint16_t in2_pin,
                GPIO_TypeDef* in3_port, uint16_t in3_pin,
                GPIO_TypeDef* in4_port, uint16_t in4_pin);

    void init();
    void setSpeed(uint8_t speed_percent);
    void drive(Direction dir);
    void stop();

private:
    TIM_HandleTypeDef* _htim_left;
    TIM_HandleTypeDef* _htim_right;
    uint32_t _ch_left;
    uint32_t _ch_right;
    GPIO_TypeDef* _in1_port; uint16_t _in1_pin;
    GPIO_TypeDef* _in2_port; uint16_t _in2_pin;
    GPIO_TypeDef* _in3_port; uint16_t _in3_pin;
    GPIO_TypeDef* _in4_port; uint16_t _in4_pin;
    uint8_t _speed;
};
