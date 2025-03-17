#include <LoRa.h>
#include "TempHumid.h"
#include "pH.h"
#include "WaterProofTemp.h"
#include "TurbiditySensor.h"
#include "TDSsensor.h"

// Define your sensor objects here
TempHumid tempHumidSensor;
pH pHSensor;
WaterProofTemp waterTempSensor;
TurbiditySensor turbiditySensor;
TDSsensor tdsSensor;

void setup() {
  Serial.begin(9600);
  // Initialize LoRa
  LoRa.begin(433E6);
  // Initialize your sensors here
  tempHumidSensor.begin();
  pHSensor.begin();
  waterTempSensor.begin();
  turbiditySensor.begin();
  tdsSensor.begin();
}

void loop() {
  // Read sensors
  float tempHumidity = tempHumidSensor.read();
  float pHValue = pHSensor.read();
  float waterTemp = waterTempSensor.read();
  float turbidity = turbiditySensor.read();
  float tdsValue = tdsSensor.read();

  // Prepare LoRa packet
  String packet = String(tempHumidity) + "," + String(pHValue) + "," +
                  String(waterTemp) + "," + String(turbidity) + "," +
                  String(tdsValue);

  // Send LoRa packet
  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();

  // Print to Serial for debugging
  Serial.println("Sent data: " + packet);

  // Delay before next reading
  delay(10000); // Adjust as needed
}
