#include "dc_motor_controller_h-bridge.h"

/**
 * @brief Constructor with enable pin.
 * 
 * @param ena Enable pin number (can be NO_PIN if unused).
 * @param inA H-bridge input A pin.alignas.
 * @param inB H-bridge input B pin.alignas.
 * @param reverse Set true to reverse motor direction.
 */
DCMotorControllerHBridge::DCMotorControllerHBridge(int8_t ena, uint8_t inA, uint8_t inB, bool reverse)
    : _pin_ena(ena),
      _pin_inA(inA),
      _pin_inB(inB),
      _reverse(reverse),
      _speed(0)
{
#if defined(ESP32)
    // Assign default PWM channels (can be made configurable in the future).
    _pwm_channel_inA = 0;
    _pwm_channel_inB = 1;
#endif
}

/**
 * @brief Constructor without enable pin.
 * 
 * @param inA H-bridge input A pin.
 * @param inB H-bridge input B pin.
 * @param reverse Set true to reverse motor direction.
 */
DCMotorControllerHBridge::DCMotorControllerHBridge(uint8_t inA, uint8_t inB, bool reverse)
    : _pin_ena(NO_PIN),
      _pin_inA(inA),
      _pin_inB(inB),
      _reverse(reverse),
      _speed(0)
{
#if defined(ESP32)
    // Assign default PWM channels (can be made configurable in the future).
    _pwm_channel_inA = 0;
    _pwm_channel_inB = 1;
#endif
}

/**
 * @brief Initializes motor control pins and configures PWM (if ESP32).
 */
void DCMotorControllerHBridge::begin() {
#if (_pin_ena != NO_PIN)
    pinMode(_pin_ena, OUTPUT);
    digitalWrite(_pin_ena, LOW);
#endif

#if defined(ESP32)
    // Configure PWM for input A.
    ledcSetup(_pwm_channel_inA, _pwm_channel_freq, _pwm_channel_reso);
    ledcAttachPin(_pin_inA, _pwm_channel_inA);

    // Configure PWM for input B.
    ledcSetup(_pwm_channel_inB, _pwm_channel_freq, _pwm_channel_reso);
    ledcAttachPin(_pin_inB, _pwm_channel_inB);
#else
    // Basic GPIO setup for non-ESP32 boards.
    pinMode(_pin_inA, OUTPUT);
    digitalWrite(_pin_inA, LOW);
    pinMode(_pin_inB, OUTPUT);
    digitalWrite(_pin_inB, LOW);
#endif
}

/**
 * @brief Deinitializes PWM channels (ESP32 only).
 */
void DCMotorControllerHBridge::end() {
#if defined(ESP32)
    ledcDetachPin(_pin_inA);
    ledcDetachPin(_pin_inB);
#endif
}

/**
 * @brief Sets the motor speed and direction.
 * 
 * @param speed_percent Speed from -100 (full reverse) to 100 (full forward).
 */
void DCMotorControllerHBridge::setSpeed(int speed_percent) {
#if (_pin_ena != NO_PIN)
    digitalWrite(_pin_ena, HIGH); // Enable motor.
#endif

    speed_percent = constrain(speed_percent, -100, 100);
    _speed = speed_percent;

    bool forward = speed_percent >= 0;
    if (_reverse) forward = !forward;

    // Convert speed percentage to PWM value.
    int pwm_value = map(abs(speed_percent), 0, 100, 0, 255);

#if defined(ESP32)
    if (speed_percent == 0) {
        ledcWrite(_pwm_channel_inA, 0);
        ledcWrite(_pwm_channel_inB, 0);
    } else if (forward) {
        ledcWrite(_pwm_channel_inA, pwm_value);
        ledcWrite(_pwm_channel_inB, 0);
    } else {
        ledcWrite(_pwm_channel_inA, 0);
        ledcWrite(_pwm_channel_inB, pwm_value);
    }
#else
    if (speed_percent == 0) {
        analogWrite(_pin_inA, 0);
        analogWrite(_pin_inB, 0);
    } else if (forward) {
        analogWrite(_pin_inA, pwm_value);
        digitalWrite(_pin_inB, LOW);
    } else {
        digitalWrite(_pin_inA, LOW);
        analogWrite(_pin_inB, pwm_value);
    }
#endif
}

/**
 * @brief Returns the last speed set via setSpeed().
 * 
 * @return int Last set speed [-100, 100].
 */
int DCMotorControllerHBridge::getCurrentSpeed() const {
    return _speed;
}

/**
 * @brief Stop the motor (motor idle stop, coasting) by setting EN to LOW.
 * 
 * Only works if the EN control pin is used.
 * If the EN control pin is not used, it is equivalent to setting the speed to 0.
 */
void DCMotorControllerHBridge::stop() {
#if (_pin_ena != NO_PIN)
    digitalWrite(_pin_ena, LOW); // Disable motor.
#endif

#if defined(ESP32)
    ledcWrite(_pwm_channel_inA, 0);
    ledcWrite(_pwm_channel_inB, 0);
#else
    digitalWrite(_pin_inA, LOW);
    digitalWrite(_pin_inB, LOW);
#endif
    _speed = 0;
}