#ifndef DC_MOTOR_CONTROLLER_H_BRIDGE_H
#define DC_MOTOR_CONTROLLER_H_BRIDGE_H

#include <Arduino.h>

/**
 * @brief A class to control a DC motor using an H-bridge driver.
 * 
 * Supports PWM control with optional enable pin and configurable direction.
 */
class DCMotorControllerHBridge {
public:

    /**
     * @brief Constructor with enable pin.
     * 
     * @param pin_ena Optional enable pin (-1 if not used).
     * @param pin_inA H-bridge input A pin.
     * @param pin_inB H-bridge input B pin.
     * @param reverse Set true to reverse motor direction.
     */
    DCMotorControllerHBridge(int8_t pin_ena, uint8_t pin_inA, uint8_t pin_inB, bool reverse);
    
    /**
     * @brief Constructor without enable pin.
     * 
     * @param pin_inA H-bridge input A pin.
     * @param pin_inB H-bridge input B pin.
     * @param reverse Set true to reverse motor direction.
     */
    DCMotorControllerHBridge(uint8_t pin_inA, uint8_t pin_inB, bool reverse);
    
    /**
     * @brief Initializes the motor control pins and PWM setup.
     * 
     * Must be called before using setSpeed().
     */
    void begin();
    
    /**
     * @brief Releases PWM channels (ESP32 only).
     * 
     * Optional cleanup when motor control is no longer needed.
     */
    void end();

    /**
     * @brief Sets the motor speed.
     * 
     * @param speed_percent Speed as a percentage from -100 (full reverse) to 100 (full forward).
     */
    void setSpeed(int speed_percent);

    /**
     * @brief Stops the motor.
     */
    void stop();

    /**
     * @brief Returns the last set speed value.
     * 
     * Note: This does not read the actual motor speed (e.g. from encoder).
     * 
     * @return int Speed value previously set by setSpeed().
     */
    int getCurrentSpeed() const;

private:
    static constexpr int8_t NO_PIN = -1; ///< Used to indicate that no enable pin is set.

    uint16_t _pwm_channel_freq = 25000; ///< Default PWM frequency in Hz.
    uint8_t _pwm_channel_reso = 8;      ///< Default PWM resolution in bits (ESP32 only).

    int8_t _pin_ena;    ///< Enable pin (optional, -1 if not used).
    uint8_t _pin_inA;   ///< Input A pin for H-bridge.
    uint8_t _pin_inB;   ///< Input B pin for H-bridge.
    int _speed;         ///< Last set speed percentage [-100, 100].
    bool _reverse;      ///< Whether to reverse motor direction.

#if defined(ESP32)
    uint8_t _pwm_channel_inA; ///< PWM channel for input A (ESP32 only).
    uint8_t _pwm_channel_inB; ///< PWM channel for input B (ESP32 only).
#endif

};

#endif  // DC_MOTOR_CONTROLLER_H_BRIDGE_H
