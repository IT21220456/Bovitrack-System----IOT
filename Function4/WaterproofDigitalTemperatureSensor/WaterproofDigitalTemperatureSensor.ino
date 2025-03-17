#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance
OneWire oneWire(ONE_WIRE_BUS);

// Pass the oneWire instance to DallasTemperature
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200); // Start serial communication
  sensors.begin();      // Initialize the sensor
  Serial.println("DS18B20 Temperature Sensor Initialized!");
}

void loop() {
  sensors.requestTemperatures(); // Send the command to get temperatures
  float temperatureC = sensors.getTempCByIndex(0); // Get temperature in Celsius

  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Sensor not detected!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");
  }

  delay(2000); // Wait 2 seconds between readings
}
