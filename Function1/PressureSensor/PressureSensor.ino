#include <Arduino.h>
#include <cmath> // Include cmath to use fmax

const int pressureSensorPin = A0; // Analog pin connected to the pressure sensor

// Calibration constants (update these based on your specific sensor and voltage conversion module)
const float voltageMin = 0.0;    // Minimum voltage output by the sensor (e.g., 0V)
const float voltageMax = 3.3;    // Maximum voltage output by the sensor (e.g., 3.3V)
const float pressureMin = 0.0;   // Minimum pressure corresponding to voltageMax (inverted, in mmHg)
const float pressureMax = 5171.49; // Maximum pressure corresponding to voltageMin (inverted, in mmHg)

void setup() {
  Serial.begin(9600); // Initialize serial monitor

  // Initialize the pressure sensor pin
  pinMode(pressureSensorPin, INPUT);
}

void loop() {
  // Read pressure sensor value
  int sensorValue = analogRead(pressureSensorPin); // Read the analog value from the pressure sensor
  float voltage = (sensorValue / 1023.0) * 3.3; // Convert the analog value to voltage (0-3.3V)

  // Convert voltage to inverted pressure in mmHg and ensure it's not negative
  float pressureMmHg = ((voltageMax - voltage) * (pressureMax - pressureMin) / (voltageMax - voltageMin)) + pressureMin;
  pressureMmHg = fmax(pressureMmHg, 0.0); // Ensure pressure is not negative

  // Print the voltage and pressure in mmHg
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V, Pressure: ");
  Serial.print(pressureMmHg);
  Serial.println(" mmHg");

  delay(3000); // Read every 3 seconds
}
