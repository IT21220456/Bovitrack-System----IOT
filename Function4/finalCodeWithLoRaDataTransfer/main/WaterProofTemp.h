#ifndef WATERPROOF_TEMP_H
#define WATERPROOF_TEMP_H

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

class WaterProofTemp {
public:
    void begin() {
        oneWire = new OneWire(ONE_WIRE_BUS);
        sensors = new DallasTemperature(oneWire);
        sensors->begin();
    }

    float read() {
        sensors->requestTemperatures();
        return sensors->getTempCByIndex(0);
    }

private:
    OneWire* oneWire;
    DallasTemperature* sensors;
};

#endif
