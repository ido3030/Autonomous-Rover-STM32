#include "MotorDriver.h"

MotorDriver::MotorDriver(
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
) : _htim_LA(htim_LA), _htim_LB(htim_LB),
    _ch_LA(ch_LA),      _ch_LB(ch_LB),
    _in1p(in1_port),    _in2p(in2_port),    _in3p(in3_port),    _in4p(in4_port),
    _in1(in1_pin),      _in2(in2_pin),      _in3(in3_pin),      _in4(in4_pin),
    _htim_RA(htim_RA),  _htim_RB(htim_RB),
    _ch_RA(ch_RA),      _ch_RB(ch_RB),
    _in5p(in5_port),    _in6p(in6_port),    _in7p(in7_port),    _in8p(in8_port),
    _in5(in5_pin),      _in6(in6_pin),      _in7(in7_pin),      _in8(in8_pin),
    _speed(0)
{}

// Read ARR per-timer so each timer can have its own period
void MotorDriver::_setPWM(TIM_HandleTypeDef* htim, uint32_t ch, uint8_t pct)
{
    uint32_t arr     = htim->Init.Period;
    uint32_t compare = ((uint32_t)pct * (arr + 1)) / 100;
    __HAL_TIM_SET_COMPARE(htim, ch, compare);
}

void MotorDriver::_setAllPWM(uint8_t pct)
{
    _setPWM(_htim_LA, _ch_LA, pct);
    _setPWM(_htim_LB, _ch_LB, pct);
    _setPWM(_htim_RA, _ch_RA, pct);
    _setPWM(_htim_RB, _ch_RB, pct);
}

void MotorDriver::_setLeftDir(GPIO_PinState a, GPIO_PinState b,
                               GPIO_PinState c, GPIO_PinState d)
{
    HAL_GPIO_WritePin(_in1p, _in1, a);
    HAL_GPIO_WritePin(_in2p, _in2, b);
    HAL_GPIO_WritePin(_in3p, _in3, c);
    HAL_GPIO_WritePin(_in4p, _in4, d);
}

void MotorDriver::_setRightDir(GPIO_PinState a, GPIO_PinState b,
                                GPIO_PinState c, GPIO_PinState d)
{
    HAL_GPIO_WritePin(_in5p, _in5, a);
    HAL_GPIO_WritePin(_in6p, _in6, b);
    HAL_GPIO_WritePin(_in7p, _in7, c);
    HAL_GPIO_WritePin(_in8p, _in8, d);
}

void MotorDriver::init()
{
    HAL_TIM_PWM_Start(_htim_LA, _ch_LA);
    HAL_TIM_PWM_Start(_htim_LB, _ch_LB);
    HAL_TIM_PWM_Start(_htim_RA, _ch_RA);
    HAL_TIM_PWM_Start(_htim_RB, _ch_RB);
    stop();
}

void MotorDriver::setSpeed(uint8_t speed_percent)
{
    if (speed_percent > 100) speed_percent = 100;
    _speed = speed_percent;
    _setAllPWM(_speed);
}

void MotorDriver::drive(MotorDirection dir)
{
    switch (dir)
    {
    case MOTOR_FORWARD:
        // IN1=H IN2=L: motor A fwd | IN3=H IN4=L: motor B fwd
        _setLeftDir(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET);
        // IN5=H IN6=L: motor C fwd | IN7=H IN8=L: motor D fwd
        _setRightDir(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET);
        _setAllPWM(_speed);
        break;

    case MOTOR_BACKWARD:
        // IN1=L IN2=H: motor A rev | IN3=L IN4=H: motor B rev
        _setLeftDir(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET);
        // IN5=L IN6=H: motor C rev | IN7=L IN8=H: motor D rev
        _setRightDir(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_SET);
        _setAllPWM(_speed);
        break;

    case MOTOR_STOP:
        stop();
        break;

    case MOTOR_BRAKE:
        brake();
        break;
    }
}

void MotorDriver::stop()
{
    _setAllPWM(0);
    _setLeftDir(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET);
    _setRightDir(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET);
}

void MotorDriver::brake()
{
    // L298N active brake: both IN pins HIGH per H-bridge, EN stays active
    _setLeftDir(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET);
    _setRightDir(GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET);
    _setAllPWM(100);
}
