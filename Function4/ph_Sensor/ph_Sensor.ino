#include <Arduino.h>

// Define the analog input pin for the pH sensor
const int pHSense = 32;  // GPIO 32 (ADC1_CH4)
const int samples = 10;
const float adc_resolution = 4095.0;  // ESP32 ADC resolution (0-4095)

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("ESP32 pH Sense");
}

float ph(float voltage) {
    return 7 + ((2.5 - voltage) / 0.18);
}

void loop() {
    int measurings = 0;

    for (int i = 0; i < samples; i++) {
        measurings += analogRead(pHSense);
        delay(10);
    }

    

    // Calculate the average voltage
    float voltage = (3.3 / adc_resolution) * (measurings / samples);
    Serial.print("pH= ");
    Serial.println(ph(voltage));
    delay(3000);

    /*
    2.5 - voltage: This part of the formula adjusts the voltage reading.
    The value 2.5 is typically used as a reference voltage that corresponds to a neutral pH of 7.

    (2.5 - voltage) / 0.18: This division scales the adjusted voltage to the pH scale. 
    The divisor 0.18 is a constant derived from the sensor's characteristics and calibration. 
    It translates the voltage difference to the corresponding pH value.

    7 + ...: This final addition converts the scaled voltage difference into a pH value, where 7 is the neutral pH.
    */
}
