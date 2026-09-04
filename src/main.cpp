#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

#include "config.hpp"
#include "secrets.hpp"

#include "mqtt/mqtt.hpp"
#include "sensors/dhtSensor.hpp"
#include "sensors/light.hpp"
#include "sensors/co2.hpp"

// --------------------------------------------------
// Sensors
// --------------------------------------------------

DHTSensor dht(
    Config::DHT_PIN,
    Config::DHT_TYPE
);

LightSensor lightSensor(
    Config::LIGHT_ADDRESS
);

Sensor* sensors[] = {
    &dht,
    &lightSensor
};

// --------------------------------------------------
// MQTT
// --------------------------------------------------

MqttClient mqtt(
    Config::MQTT_BROKER,
    Config::MQTT_PORT,
    Config::DEVICE_ID,
    Config::MQTT_TOPIC_PREFIX
);

// --------------------------------------------------
// Timing
// --------------------------------------------------

unsigned long lastSensorUpdate = 0;

// --------------------------------------------------
// WiFi
// --------------------------------------------------

void connectWiFi()
{
    Serial.print("Connecting to WiFi");

    WiFi.begin(
        Secrets::WIFI_SSID,
        Secrets::WIFI_PASSWORD
    );

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

// --------------------------------------------------
// Setup
// --------------------------------------------------

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("ESP32 Sensor starting...");

    // WiFi
    connectWiFi();

    // MQTT
    mqtt.begin();
    mqtt.connect();

    Wire.begin(Config::SDA, Config::SCL);
    byte error;

    Serial.println("Scanning I2C...");

    for (byte address = 1; address < 127; address++)
    {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print("I2C device found at 0x");
            
            if (address < 16)
            {
                Serial.print("0");
            }

            Serial.println(address, HEX);
        }
    }

    Serial.println("I2C scan complete");

    // Sensors
    for (Sensor* sensor : sensors)
    {
        if (!sensor->begin())
        {
            Serial.print("Sensor initialization failed for ");
            Serial.println(sensor->sensorName());
        }
    }


    Serial.println("Setup complete");
}

// --------------------------------------------------
// Loop
// --------------------------------------------------

void loop()
{
    mqtt.loop();

    if (!mqtt.connected())
    {
        mqtt.connect();
    }

    // Update sensors at the configured interval
    const unsigned long now = millis();

    if (now - lastSensorUpdate >= Config::SENSOR_INTERVAL_MS)
    {
        lastSensorUpdate = now;

        for (Sensor* sensor : sensors)
        {
            if (!sensor->isInitialized())
            {
                Serial.print("Sensor not initialized: ");
                Serial.println(sensor->sensorName());

                if (!sensor->begin())
                {
                    continue;
                }
            }

            if (sensor->update())
            {
                mqtt.publish(
                    *sensor,
                    sensor->sensorName()
                );
            }
            else
            {
                Serial.print("Failed to read ");
                Serial.println(sensor->sensorName());
            }
        }
    }
}