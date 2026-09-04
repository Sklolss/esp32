#pragma once

#include <sensor.hpp>
#include <Arduino.h>
#include <BH1750.h>

class LightSensor : public Sensor {
    public:
        LightSensor(
            uint8_t _address = 0x23
        );
        
        bool begin() override;
        bool update() override;

        const char* sensorName() const override {
            return "BH1750";
        }

        std::size_t measurements(
            Measurement* buffer,
            std::size_t maxMeasurements
        ) const override;

        private:
            BH1750 _sensor;
            uint8_t _address;

            float _lightLevel = NAN;
};