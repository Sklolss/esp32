#pragma once

#include <cstdint>

struct Measurement {
    const char* name;
    float value;
};

class Sensor {
public:
    virtual ~Sensor() = default;

    // Initialize the physical sensor.
    virtual bool begin() = 0;

    // Read the sensor and update its internal values.
    // Returns true if the reading was successful.
    virtual bool update() = 0;

    virtual const char* sensorName() const = 0;

    bool isInitialized() const {
        return _initialized;
    }

    /**
     * Write the current measurements into the supplied buffer.
     *
     * Returns the number of measurements written.
     */
    virtual std::size_t measurements(
        Measurement* buffer,
        std::size_t maxMeasurements
    ) const = 0;

    protected:
        bool _initialized = false;
};