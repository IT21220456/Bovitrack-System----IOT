#ifndef TDS_SENSOR_H
#define TDS_SENSOR_H

#include <Arduino.h>

#define TDS_SENSOR_PIN 33 // Use GPIO33 (ADC1_CHANNEL_5) as the analog pin

class TDSsensor {
public:
    void begin() {
        pinMode(TDS_SENSOR_PIN, INPUT);
    }

    float read() {
        int sensorValue = analogRead(TDS_SENSOR_PIN);
        float voltage = sensorValue * (3.3 / 4095.0); // ESP32 ADC resolution is 12 bits, 3.3V reference
        float tdsValue = (133.42 * sq(voltage) - 255.86 * voltage + 857.39) * 0.5; // Example formula
        return tdsValue;
    }
};

#endif
