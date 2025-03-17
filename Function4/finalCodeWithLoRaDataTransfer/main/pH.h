#ifndef PH_H
#define PH_H

class pH {
public:
    void begin() {
        // Initialize pH sensor if needed
    }

    float read() {
        int sensorValue = analogRead(A0);
        float voltage = sensorValue * (5.0 / 1023.0);
        float pHValue = 3.5 * voltage + 0.5; // Example conversion formula
        return pHValue;
    }
};

#endif
