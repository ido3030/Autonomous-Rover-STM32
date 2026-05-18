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
    MotorDriver(
        TIM_HandleTypeDef* htim_left,  uint32_t ch_left,
        TIM_HandleTypeDef* htim_right, uint32_t ch_right,
        GPIO_TypeDef* in1_port, uint16_t in1_pin,
        GPIO_TypeDef* in2_port, uint16_t in2_pin,
        GPIO_TypeDef* in3_port, uint16_t in3_pin,
        GPIO_TypeDef* in4_port, uint16_t in4_pin
    );

    void init();
    void setSpeed(uint8_t speed_percent);
    void drive(MotorDirection dir);
    void stop();
    void brake();

private:
    TIM_HandleTypeDef* _htim_L;
    TIM_HandleTypeDef* _htim_R;
    uint32_t _ch_L, _ch_R;
    GPIO_TypeDef *_in1p, *_in2p, *_in3p, *_in4p;
    uint16_t _in1, _in2, _in3, _in4;
    uint8_t _speed;
    uint32_t _arr;

    void _setPWM(TIM_HandleTypeDef* htim, uint32_t ch, uint8_t pct);
};

#endif
