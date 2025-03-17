#define ECG_SIGNAL_PIN 35
#define LO_PLUS_PIN 14
#define LO_MINUS_PIN 12

void setupECGSensor() {
  pinMode(ECG_SIGNAL_PIN, INPUT);
  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);
}

String readECG() {
  if (digitalRead(LO_PLUS_PIN) == HIGH || digitalRead(LO_MINUS_PIN) == HIGH) {
    return "Lead Off Detected!";
  } else {
    int ecgSignal = analogRead(ECG_SIGNAL_PIN);
    return "ECG:" + String(ecgSignal);
  }
}
