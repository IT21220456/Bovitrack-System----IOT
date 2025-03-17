#define ADC_VREF_mV 3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0
#define PIN_LM35 36 // ESP32 pin GPIO36 (ADC0) connected to LM35

void setup() {
  Serial.begin(9600); // initialize serial communication
  analogSetAttenuation(ADC_11db); // set the ADC attenuation to 11 dB (up to ~3.3V input)
}

void loop() {
  int adcVal = analogRead(PIN_LM35); // read the ADC value from the temperature sensor
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION); // convert the ADC value to voltage in millivolt
  float tempC = milliVolt / 10; // convert the voltage to temperature in °C
  float tempF = tempC * 9 / 5 + 32; // convert the °C to °F

  Serial.print("Temperature: ");
  Serial.print(tempC); // print the temperature in °C
  Serial.print("°C ~ "); // separator between °C and °F
  Serial.print(tempF); // print the temperature in °F
  Serial.println("°F"); // print a new line
  
  delay(500); // wait for 500ms
}
