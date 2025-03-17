#include <Arduino.h>

// Define the PWM pin for the humidifier signal
const int humidifierPin = 25;  // GPIO 25

// Ultrasonic humidifier frequency
const int frequency = 113000;  // 113kHz

void setup() {
    pinMode(humidifierPin, OUTPUT);
    ledcSetup(0, frequency, 8);  // Setup LEDC timer with 113kHz frequency and 8-bit resolution
    ledcAttachPin(humidifierPin, 0);  // Attach the humidifier pin to the LEDC channel
    ledcWrite(0, 128);  // Start with a moderate duty cycle
}

void loop() {
    // You can adjust the duty cycle here to control the mist output
    ledcWrite(0, 128);  // Example: 50% duty cycle
    delay(1000);  // Run for 1 second
    ledcWrite(0, 0);  // Turn off for 1 second
    delay(1000);
}
