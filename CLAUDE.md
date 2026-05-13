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
- Meeting 1-2: LED blink, button input, OpenOCD debug setup
- Meeting 3: Bidirectional UART (W/A/S/D commands + telemetry)

## Next task — Meeting 4: PWM Motor Control
- Configure Timer in CubeMX for PWM output
- Write PWM driver class
- Test on green LED (Fade in/out) before connecting motors
- Validate signal with DSLogic

## Branching strategy
- main = stable tested code only
- feature/pwm-motors = current work
- Always commit to feature branch, merge to main after DSLogic validation
