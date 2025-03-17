#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <SPI.h>
#include <LoRa.h>
#include <cmath> // Include cmath to use fmax

// GPS Configuration
#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

// Pressure Sensor Configuration
const int pressureSensorPin = A0; // Analog pin connected to the pressure sensor
const float voltageMin = 0.0;    // Minimum voltage output by the sensor (e.g., 0V)
const float voltageMax = 3.3;    // Maximum voltage output by the sensor (e.g., 3.3V)
const float pressureMin = 0.0;   // Minimum pressure corresponding to voltageMax (inverted, in mmHg)
const float pressureMax = 5171.49; // Maximum pressure corresponding to voltageMin (inverted, in mmHg)

// LoRa Configuration
#define BAND 433E6  // LoRa frequency band (433MHz)
#define SCK 18
#define MISO 19
#define MOSI 23
#define SS 5
#define RST 14
#define DIO0 26

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Initialize GPS
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  
  // Initialize Pressure Sensor Pin
  pinMode(pressureSensorPin, INPUT);

  // Initialize LoRa
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa initialized successfully");
}

void loop() {
  // Read pressure sensor value
  int sensorValue = analogRead(pressureSensorPin); // Read the analog value from the pressure sensor
  float voltage = (sensorValue / 1023.0) * 3.3; // Convert the analog value to voltage (0-3.3V)

  // Convert voltage to inverted pressure in mmHg and ensure it's not negative
  float pressureMmHg = ((voltageMax - voltage) * (pressureMax - pressureMin) / (voltageMax - voltageMin)) + pressureMin;
  pressureMmHg = fmax(pressureMmHg, 0.0); // Ensure pressure is not negative

  // Read GPS data
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    String payload = "Voltage=" + String(voltage, 2) + "V,Pressure=" + String(pressureMmHg, 2) + "mmHg,";
    payload += "LAT=" + String(gps.location.lat(), 6) + ",LONG=" + String(gps.location.lng(), 6);
    payload += ",SPEED=" + String(gps.speed.kmph()) + "km/h,ALT=" + String(gps.altitude.meters()) + "m";
    payload += ",HDOP=" + String(gps.hdop.value() / 100.0) + ",Satellites=" + String(gps.satellites.value());
    payload += ",Time=" + String(gps.date.year()) + "/" + String(gps.date.month()) + "/" + String(gps.date.day());
    payload += "," + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());

    // Send packet
    Serial.println("Sending packet: " + payload);
    LoRa.beginPacket();
    LoRa.print(payload);
    LoRa.endPacket();
  }

  delay(5000); // Send data every 5 seconds
}
