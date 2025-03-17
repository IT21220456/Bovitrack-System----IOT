#ifndef TEMP_HUMID_H
#define TEMP_HUMID_H

#include "DHT.h"

#define DHTPIN 2     // Pin where the DHT22 is connected
#define DHTTYPE DHT22

class TempHumid {
public:
    void begin() {
        dht.begin();
    }

    float read() {
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        return t;
    }

private:
    DHT dht = DHT(DHTPIN, DHTTYPE);
};

#endif
