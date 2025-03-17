#include <Wire.h>
#include <MAX30100.h>

MAX30100 pulseoximeter;

void setup() {
    Serial.begin(115200);
    pulseoximeter.begin();
    pulseoximeter.setPulseLED(1, 0); // Set the pulse LED to red
    pulseoximeter.setPulseLED(0, 1); // Set the SpO2 LED to IR
    pulseoximeter.setMode(0x01); // Set to pulse mode
}

void loop() {
    if (pulseoximeter.readHeartRate() && pulseoximeter.readSpO2()) {
        Serial.print("Heart Rate: ");
        Serial.print(pulseoximeter.getHeartRate());
        Serial.println(" BPM");
        Serial.print("SpO2: ");
        Serial.print(pulseoximeter.getSpO2());
        Serial.println(" %");
    }
    delay(1000);
}
