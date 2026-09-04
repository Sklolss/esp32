#pragma once

#include <Arduino.h>

namespace Config {

    // Device
    constexpr const char* DEVICE_ID = "bedroom";

    // MQTT
    constexpr const char* MQTT_BROKER = "192.168.0.3";
    constexpr uint16_t MQTT_PORT = 1883;
    constexpr const char* MQTT_TOPIC_PREFIX = "sensors";

    // I2C
    constexpr uint8_t SDA = 21;
    constexpr uint8_t SCL = 22;

    // DHT22
    constexpr uint8_t DHT_PIN = 15;
    constexpr uint8_t DHT_TYPE = 22;

    // LightSensor (BH1750)
    constexpr uint8_t LIGHT_ADDRESS = 0x23; //Depends where ADD is connected to. 0x23 for ground, 0x5c for VCC


    // Sensor update interval
    constexpr uint32_t SENSOR_INTERVAL_MS = 10000;
}