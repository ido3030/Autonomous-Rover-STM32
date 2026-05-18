#include "MotorDriver.h"

MotorDriver::MotorDriver(
    TIM_HandleTypeDef* htim_left,  uint32_t ch_left,
    TIM_HandleTypeDef* htim_right, uint32_t ch_right,
    GPIO_TypeDef* in1_port, uint16_t in1_pin,
    GPIO_TypeDef* in2_port, uint16_t in2_pin,
    GPIO_TypeDef* in3_port, uint16_t in3_pin,
    GPIO_TypeDef* in4_port, uint16_t in4_pin
) : _htim_L(htim_left),
    _htim_R(htim_right),
    _ch_L(ch_left),  _ch_R(ch_right),
    _in1p(in1_port), _in2p(in2_port), _in3p(in3_port), _in4p(in4_port),
    _in1(in1_pin),   _in2(in2_pin),   _in3(in3_pin),   _in4(in4_pin),
    _speed(0), _arr(0)
{}

void MotorDriver::init()
{
    _arr = _htim_L->Init.Period;
    HAL_TIM_PWM_Start(_htim_L, _ch_L);
    HAL_TIM_PWM_Start(_htim_R, _ch_R);
    stop();
}

void MotorDriver::_setPWM(TIM_HandleTypeDef* htim, uint32_t ch, uint8_t pct)
{
    uint32_t compare = ((uint32_t)pct * (_arr + 1)) / 100;
    __HAL_TIM_SET_COMPARE(htim, ch, compare);
}

void MotorDriver::setSpeed(uint8_t speed_percent)
{
    if (speed_percent > 100) speed_percent = 100;
    _speed = speed_percent;
    _setPWM(_htim_L, _ch_L, _speed);
    _setPWM(_htim_R, _ch_R, _speed);
}

void MotorDriver::drive(MotorDirection dir)
{
    switch (dir)
    {
    case MOTOR_FORWARD:
        HAL_GPIO_WritePin(_in1p, _in1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(_in2p, _in2, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_in3p, _in3, GPIO_PIN_SET);
        HAL_GPIO_WritePin(_in4p, _in4, GPIO_PIN_RESET);
        break;
    case MOTOR_BACKWARD:
        HAL_GPIO_WritePin(_in1p, _in1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_in2p, _in2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(_in3p, _in3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_in4p, _in4, GPIO_PIN_SET);
        break;
    case MOTOR_STOP:
        HAL_GPIO_WritePin(_in1p, _in1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_in2p, _in2, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_in3p, _in3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(_in4p, _in4, GPIO_PIN_RESET);
        break;
    case MOTOR_BRAKE:
        HAL_GPIO_WritePin(_in1p, _in1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(_in2p, _in2, GPIO_PIN_SET);
        HAL_GPIO_WritePin(_in3p, _in3, GPIO_PIN_SET);
        HAL_GPIO_WritePin(_in4p, _in4, GPIO_PIN_SET);
        break;
    }
    _setPWM(_htim_L, _ch_L, _speed);
    _setPWM(_htim_R, _ch_R, _speed);
}

void MotorDriver::stop()
{
    _setPWM(_htim_L, _ch_L, 0);
    _setPWM(_htim_R, _ch_R, 0);
    HAL_GPIO_WritePin(_in1p, _in1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_in2p, _in2, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_in3p, _in3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(_in4p, _in4, GPIO_PIN_RESET);
}

void MotorDriver::brake()
{
    _setPWM(_htim_L, _ch_L, 0);
    _setPWM(_htim_R, _ch_R, 0);
    HAL_GPIO_WritePin(_in1p, _in1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(_in2p, _in2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(_in3p, _in3, GPIO_PIN_SET);
    HAL_GPIO_WritePin(_in4p, _in4, GPIO_PIN_SET);
}
