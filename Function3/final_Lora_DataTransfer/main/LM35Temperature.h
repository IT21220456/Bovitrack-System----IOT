#define TEMP_SENSOR_PIN 34

void setupTemperatureSensor() {
  pinMode(TEMP_SENSOR_PIN, INPUT);
}

String readTemperature() {
  float temperature = analogRead(TEMP_SENSOR_PIN) * (3.3 / 4095.0) * 100;
  return "Temp:" + String(temperature) + "C";
}
