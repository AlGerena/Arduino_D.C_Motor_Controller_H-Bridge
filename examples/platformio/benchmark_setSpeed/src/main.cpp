/**
 * @file benchmark_setSpeed.cpp
 * @brief Benchmarking the execution time of DCMotorControllerHBridge::setSpeed()
 * 
 * Demonstrates how to measure and print the time it takes to call setSpeed()
 * using the Arduino `micros()` function.
 */

#include <Arduino.h>
#include <dc_motor_controller_h-bridge.h>

// Create motor instance on pins IN1=13, IN2=14, no enable pin, normal direction
DCMotorControllerHBridge motor(13, 14, false);

void setup() {
  Serial.begin(115200);
  while (!Serial) { /* wait for Serial */ }

  motor.begin();  // Initialize pins and PWM
  Serial.println("Starting setSpeed() benchmark...");
}

void loop() {
  // Measure time before calling setSpeed()
  unsigned long startTime = micros();

  motor.setSpeed(-75);  // Set speed to 75% reverse

  // Measure time after calling setSpeed()
  unsigned long endTime = micros();

  // Print elapsed time in microseconds
  Serial.print("setSpeed() execution time: ");
  Serial.print(endTime - startTime);
  Serial.println(" µs");

  motor.setSpeed(0);  // Reset speed to zero (brake)
  delay(500);         // Wait 0.5 seconds
}
