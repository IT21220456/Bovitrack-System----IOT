#ifndef TURBIDITY_SENSOR_H
#define TURBIDITY_SENSOR_H

#include <Arduino.h>

#define TURBIDITY_SENSOR_PIN 32 // Use GPIO32 (ADC1_CHANNEL_4) as the analog pin

class TurbiditySensor {
public:
    void begin() {
        pinMode(TURBIDITY_SENSOR_PIN, INPUT);
    }

    float read() {
        int sensorValue = analogRead(TURBIDITY_SENSOR_PIN);
        float voltage = sensorValue * (3.3 / 4095.0); // ESP32 ADC resolution is 12 bits, 3.3V reference
        float turbidity = -1120.4 * sq(voltage) + 5742.3 * voltage - 4352.9; // Example formula
        return turbidity;
    }
};

#endif
