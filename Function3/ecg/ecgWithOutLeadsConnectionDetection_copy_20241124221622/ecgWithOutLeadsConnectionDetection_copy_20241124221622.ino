#define ECG_PIN 34 // Analog pin connected to AD8232 OUTPUT (use your chosen ADC pin)

void setup() {
  Serial.begin(115200); // Start the Serial Monitor at 115200 baud
  pinMode(ECG_PIN, INPUT); // Set ECG_PIN as an input
  Serial.println("AD8232 ECG Monitor Initialized");
}

void loop() {
  int ecgValue = analogRead(ECG_PIN); // Read analog signal from AD8232
  Serial.println(ecgValue); // Print the raw value to the Serial Monitor
  delay(100); // Small delay for smoother readings
}
