# Rover Project — Architect Brief for Claude Code

## Your role
You are the SOFTWARE ENGINEER. Claude in chat is the ARCHITECT.
Always read this file before writing any code.

## Project
Autonomous rover on STM32 Nucleo-F746ZG.
Future: Raspberry Pi companion computer + camera for object detection.

## Hardware
- Board: STM32 Nucleo-F746ZG
- Motor driver: L298N (arriving soon)
- Motors: 4x DC motors (4WD chassis)
- Servo: SG90 (for ultrasonic sensor rotation)
- Sensor: HC-SR04 ultrasonic
- Power: 6x AA (9V) → LM2596 buck converter → 5V to Nucleo
- Tools: DSLogic logic analyzer for PWM validation

## Architecture rules
- Language: C++ only on Nucleo
- Pattern: layered — Drivers → Services → Application
- Every driver = its own class
- Common GND bus — never mix power rails
- Servo powered from buck converter, NOT from Nucleo pins

## Completed so far
- Meeting 1-2: LED blink, button, OpenOCD debug
- Meeting 3: Bidirectional UART telemetry
- Meeting 4: PWM on PB7/TIM4_CH2, DSLogic validated 1kHz
- Meeting 5: W/A/S/D UART control + MotorDriver.h/.cpp created

## Next task — Meeting 6: Connect L298N hardware
- Hardware arriving soon
- Wire L298N using MotorDriver pin map
- Test motors with DSLogic on ENA/ENB pins first
- Flash MotorDriver into main.c and drive motors

## Flash command
openocd -f interface/stlink.cfg -f target/stm32f7x.cfg -c "program build/Debug/Nucleo_First_Blink.elf verify reset exit"

## Branching strategy
- main = stable tested code only
- feature/pwm-motors = current work
- Always commit to feature branch, merge to main after DSLogic validation
