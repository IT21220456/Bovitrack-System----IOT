// Define the analog input pin for the ECG signal
const int ECG_PIN = 32;  // GPIO 32 (ADC1_CH4)

// Pins for Lead Off detection
const int LO_PLUS_PIN = 26;  // GPIO 26
const int LO_MINUS_PIN = 25;  // GPIO 25

void setup() {
  Serial.begin(115200);
  
  // Initialize pin modes
  pinMode(ECG_PIN, INPUT);
  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);
}

void loop() {
  // Check Lead Off Detection
  bool leadOffDetected = false;
  if (digitalRead(LO_PLUS_PIN) == HIGH || digitalRead(LO_MINUS_PIN) == HIGH) {
    leadOffDetected = true;
    Serial.println("Leads off detected");
  } else {
    Serial.println("Leads connected");
  }

  // Read the ECG signal if leads are connected
  if (!leadOffDetected) {
    int ecgValue = analogRead(ECG_PIN);
    Serial.print("ECG Value: ");
    Serial.println(ecgValue);
  } else {
    Serial.println("ECG Value: not read due to lead off");
  }

  // Small delay for stability
  delay(500);
}
