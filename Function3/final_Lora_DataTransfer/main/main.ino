#include "Pulse_oximeter.h"
#include "LM35Temperature.h"
#include "ECG.h"
#include <LoRa.h>

void setup() {
  Serial.begin(115200);

  // Initialize sensors
  setupPulseOximeter();
  setupTemperatureSensor();
  setupECGSensor();

  // Initialize LoRa
  LoRa.begin(433E6);
  LoRa.setTxPower(20, 0);
}

void loop() {
  String pulseOximeterData = readPulseOximeter();
  String temperatureData = readTemperature();
  String ecgData = readECG();

  // Combine all sensor data
  String data = pulseOximeterData + ", " + temperatureData + ", " + ecgData;

  // Send data via LoRa
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();

  Serial.println(data); // For debugging

  delay(1000); // Delay before next reading
}
