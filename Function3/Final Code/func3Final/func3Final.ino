#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

PulseOximeter pox;
uint32_t tsLastReport = 0;

#define TEMP_SENSOR_PIN 34
#define ECG_SIGNAL_PIN 35
#define LO_PLUS_PIN 14
#define LO_MINUS_PIN 12

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the Pulse Oximeter sensor
  if (!pox.begin()) {
    Serial.println("Failed to initialize pulse oximeter sensor");
    while (1);
  }
  
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Initialize temperature sensor
  pinMode(TEMP_SENSOR_PIN, INPUT);

  // Initialize ECG sensor lead-off detection pins
  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);
}

void loop() {
  // Make sure to call update as fast as possible
  pox.update();
  
  // Check the time interval for reporting
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    // Pulse rate and SpO2 are measured here
    Serial.print("Pulse Rate: ");
    Serial.print(pox.getHeartRate());
    Serial.print(" bpm, ");
    
    Serial.print("SpO2: ");
    Serial.print(pox.getSpO2());
    Serial.println(" %");

    tsLastReport = millis();
  }

  // Read temperature
  float temperature = analogRead(TEMP_SENSOR_PIN) * (3.3 / 4095.0) * 100;

  // Read ECG signal
  int ecgSignal = analogRead(ECG_SIGNAL_PIN);

  // Check lead-off detection
  if (digitalRead(LO_PLUS_PIN) == HIGH || digitalRead(LO_MINUS_PIN) == HIGH) {
    Serial.println("Lead Off Detected!");
  } else {
    Serial.print("ECG Signal: ");
    Serial.println(ecgSignal);
  }

  // Print temperature
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Wait before the next reading
  delay(1000);
}
