# DCMotorControllerHBridge

A simple and efficient Arduino-compatible C++ library for controlling a DC motor using an H-bridge driver (such as L298, L293, or compatible).
Designed for compatibility with both ESP32 (via `ledc`) and standard Arduino boards (`analogWrite`).

This library allows precise control of motor direction and speed using PWM signals, and supports H-bridges with or without an enable (`EN`) pin.

## Features

- Supports H-bridges with or without enable pin (`EN`), e.g. L298N, L293D, or compatible.
- PWM control for speed.
- Direction inversion support.
- Compatible with ESP32 (hardware PWM via `ledc`) and standard Arduino `analogWrite`.
- Optional enable pin control for motor activation.
- Lightweight and fast (e.g., `setSpeed()` takes ~9µs on ESP32).
- Easily modifiable default frequency and resolution.

## Usage

### Constructor options

```cpp
// With enable pin.
DCMotorControllerHBridge motor(pin_ena, pin_inA, pin_inB, reverse);

// Without enable pin.
DCMotorControllerHBridge motor(pin_inA, pin_inB, reverse);
```

- `pin_ena` (int8_t): Pin connected to the enable (EN) input of the H-bridge (set to -1 or omit if not used).
- `pin_inA`, `pin_inB` (uint8_t): Pins connected to the input control signals, IN1 (inA) and IN2 (inB) of the H-bridge.
- `reverse` (bool): Set to true to reverse the motor direction logic.

### Setup

```cpp
motor.begin();  // Initialize motor pins and PWM.
```

### Set motor speed

```cpp
motor.setSpeed(75);    // Forward at 75% speed.
motor.setSpeed(-100);  // Reverse at full speed.
motor.setSpeed(0);     // Stop the motor (quick motor stop, brake) by simultaneously setting both input controls, IN1 and IN2, to LOW/HIGH.
```

### Stop motor

```cpp
motor.stop();   // Stop the motor (motor idle stop, coasting) by setting EN to LOW. 
                // Only works if the EN control pin is used. 
                // If the EN control pin is not used, it is equivalent to setting the speed to 0.
```

### Get current speed

```cpp
int speed = motor.getCurrentSpeed();  // Returns last speed set (-100 to 100).
```

## PlatformIO Example For ESP32 Using Arduino Framework

An example sketch demonstrating basic usage of the `DCMotorControllerHBridge` library is available in the `examples/platformio/benchmark_setSpeed` directory of this repository.

## Customization

You can customize the default PWM frequency and resolution (for ESP32) by modifying these members in the class:

```cpp
uint16_t _pwm_channel_freq = 25000; // Hz
uint8_t  _pwm_channel_reso = 8;     // bits
```

> ⚠️ The ESP32’s `ledc` peripheral does not support every frequency–resolution combination.  
> For instance, **25 kHz @ 8 bits** works reliably, whereas **25 kHz @ 16 bits** will fail to initialize.  
>
> ℹ️ Common H-bridge drivers (e.g., L298N, L293D, and compatibles) specify a maximum, typical, and sometimes minimum switching frequency in their datasheets.  
> For example, the L298N lists a **maximum** of 40 kHz, a **typical** of 25 kHz, and no defined minimum. 
> I recommend using the **typical** frequency or **never exceeding** the maximum.

## License

MIT License

## Author

**agerena**