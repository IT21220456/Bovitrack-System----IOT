#define FSR_PIN 34 // Analog pin connected to AO pin of the module

// Maximum weight the FSR sensor can measure (adjust based on calibration)
#define MAX_WEIGHT_KG 10.0
#define MAX_ANALOG_READING 4095.0 // Maximum ADC value (for ESP32)

void setup() {
  Serial.begin(115200); // Initialize Serial Communication
  Serial.println("FSR Sensor Reading (Weight in kg) Started...");
}

void loop() {
  int sensorValue = analogRead(FSR_PIN); // Read the raw analog value

  // Invert the raw sensor reading (subtract from MAX_ANALOG_READING)
  int invertedValue = MAX_ANALOG_READING - sensorValue;

  // Map the inverted value to weight in kg
  float weightInKg = (invertedValue / MAX_ANALOG_READING) * MAX_WEIGHT_KG;

  // Display the raw, inverted value and weight
  Serial.print("Raw Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Inverted Value: ");
  Serial.print(invertedValue);
  Serial.print(" | Approx. Weight: ");
  Serial.print(weightInKg, 2); // Display with 2 decimal places
  Serial.println(" kg");

  delay(500); // Update every 500ms for smoother readings
}
