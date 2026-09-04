#include "mqtt.hpp"

#include "secrets.hpp"

MqttClient::MqttClient(
    const char* broker,
    uint16_t port,
    const char* location,
    const char* topicPrefix
)
    : _mqttClient(_wifiClient),
      _broker(broker),
      _port(port),
      _location(location),
      _topicPrefix(topicPrefix) {
}

void MqttClient::begin() {
    _mqttClient.setServer(_broker, _port);
}

bool MqttClient::connect() {
    if (_mqttClient.connected()) {
        return true;
    }

    Serial.print("Connecting to MQTT broker... ");

    bool success;

    if (Secrets::MQTT_USERNAME[0] != '\0') {
        success = _mqttClient.connect(
            _location,
            Secrets::MQTT_USERNAME,
            Secrets::MQTT_PASSWORD
        );
    } else {
        success = _mqttClient.connect(_location);
    }

    if (success) {
        Serial.println("connected");
        return true;
    }

    Serial.print("failed, state=");
    Serial.println(_mqttClient.state());

    return false;
}

void MqttClient::loop() {
    if (!_mqttClient.connected()) {
        return;
    }

    _mqttClient.loop();
}

bool MqttClient::connected() {
    return _mqttClient.connected();
}

bool MqttClient::publish(
    const Sensor& sensor,
    const char* sensorName
) {
    if (!_mqttClient.connected()) {
        return false;
    }

    constexpr std::size_t MAX_MEASUREMENTS = 8;

    Measurement measurements[MAX_MEASUREMENTS];

    const std::size_t meassurementsArray = sensor.measurements(
        measurements,
        MAX_MEASUREMENTS
    );

    bool success = true;

    for (std::size_t i = 0; i < meassurementsArray; ++i) {
        char topic[128];
        char message[32];

        // Topic used for mqtt message, format: <topicPrefix>/<location>/<measurementName> ex: sensors/bedroom/temperature
        snprintf(
            topic,
            sizeof(topic),
            "%s/%s/%s",
            _topicPrefix,
            _location,
            measurements[i].name
        );

        // message sent using mqtt
        snprintf(
            message,
            sizeof(message),
            "%.2f",
            measurements[i].value
        );

        // Debug print to terminal of esp
        Serial.printf(
            "%s %.2f\n",
            measurements[i].name,
            measurements[i].value
        );

        const bool published = _mqttClient.publish(
            topic,
            message,
            true
        );

        if (!published) {
            success = false;
        }
    }

    return success;
}