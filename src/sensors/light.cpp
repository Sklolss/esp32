#include "light.hpp"

LightSensor::LightSensor(uint8_t address)
    : _address(address) {
}

bool LightSensor::begin()
{
    

    _initialized = _sensor.begin(
        BH1750::CONTINUOUS_HIGH_RES_MODE,
        _address,
        &Wire
    );

    if (!_initialized) {
        Serial.println("LightSensor (BH1750) initialization failed");
        return false;
    }

    Serial.println("LightSensor (BH1750) initialized");

    return true;
}

bool LightSensor::update()
{
    if(!_initialized) {
        return false;
    }

    const float lightLevel = _sensor.readLightLevel();

    if (isnan(lightLevel) || lightLevel < 0) {
        Serial.println("LightSensor (BH1750) reading failed");
        return false;
    }

    _lightLevel = lightLevel;
    return true;
}

std::size_t LightSensor::measurements(
    Measurement* buffer,
    std::size_t maxMeasurements
) const {
    if (buffer == nullptr || maxMeasurements < 1) {
        return 0;
    }

    buffer[0] = {
        "light",
        _lightLevel
    };

    return 1;
}