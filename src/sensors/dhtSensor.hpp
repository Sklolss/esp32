#pragma once

#include <sensor.hpp>
#include <Arduino.h>
#include <DHT.h>

class DHTSensor : public Sensor {
    public:
        DHTSensor(uint8_t pin, uint8_t type);

        bool begin() override;
        bool update() override;

        const char* sensorName() const override {
            return "DHT22";
        }

        std::size_t measurements(
            Measurement* buffer,
            std::size_t maxMeasurements
        ) const override;

    private:
        DHT _dht;

        float _temperature = NAN;
        float _humidity = NAN;

};