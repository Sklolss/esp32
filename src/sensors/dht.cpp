#include "dhtSensor.hpp"

DHTSensor::DHTSensor(uint8_t pin, uint8_t type)
    : _dht(pin, type) {
}

bool DHTSensor::begin() {
    _dht.begin();

    _initialized = true;
    return _initialized;
}

bool DHTSensor::update() {
    float temperature = _dht.readTemperature();
    float humidity = _dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        return false;
    }

    _temperature = temperature;
    _humidity = humidity;

    return true;
}

std::size_t DHTSensor::measurements(
    Measurement* buffer,
    std::size_t maxMeasurements
) const {
    if (buffer == nullptr || maxMeasurements < 2) {
        return 0;
    }

    buffer[0] = {
        "temperature",
        _temperature
    };

    buffer[1] = {
        "humidity",
        _humidity
    };

    return 2;
}