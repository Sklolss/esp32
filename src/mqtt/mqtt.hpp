#pragma once

#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#include "../sensor.hpp"

class MqttClient {
public:
    MqttClient(
        const char* broker,
        uint16_t port,
        const char* location,
        const char* topicPrefix = "sensors"
    );

    void begin();

    bool connect();
    void loop();

    bool connected();

    bool publish(
        const Sensor& sensor,
        const char* sensorName
    );

private:
    WiFiClient _wifiClient;
    PubSubClient _mqttClient;

    const char* _broker;
    uint16_t _port;
    const char* _location;
    const char* _topicPrefix;
};