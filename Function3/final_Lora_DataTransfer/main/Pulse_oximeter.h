#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

PulseOximeter pox;
uint32_t tsLastReport = 0;

void setupPulseOximeter() {
  if (!pox.begin()) {
    Serial.println("Failed to initialize pulse oximeter sensor");
    while (1);
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
}

String readPulseOximeter() {
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    float heartRate = pox.getHeartRate();
    float spO2 = pox.getSpO2();
    tsLastReport = millis();
    return "HR:" + String(heartRate) + ", SpO2:" + String(spO2);
  }
  return "";
}
