/**
 * EdgeConductor — ESP8266 / NodeMCU
 * ===================================
 * Board: NodeMCU v2 / Wemos D1 Mini / ESP-12E
 * PlatformIO env: nodemcuv2
 *
 * Setup:
 *   1. Copy secrets.example.h → secrets.h (same folder), fill in credentials
 *   2. pio run -e nodemcuv2 --target upload
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "secrets.h"

WiFiClientSecure net;
PubSubClient     mqtt(net);

char topicTelemetry[80];
char topicConfig[80];
char topicOTA[80];

void onMessage(char* topic, byte* payload, unsigned int len) {
    char buf[256];
    memcpy(buf, payload, min(len, (unsigned)255));
    buf[min(len, (unsigned)255)] = '\0';
    Serial.printf("[EC] %s → %s\n", topic, buf);
}

void connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.printf("[EC] WiFi %s", WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.printf(" connected (%s)\n", WiFi.localIP().toString().c_str());
}

void connectMQTT() {
    net.setInsecure();
    mqtt.setServer(MQTT_HOST, MQTT_PORT);
    mqtt.setCallback(onMessage);
    while (!mqtt.connected()) {
        Serial.printf("[EC] MQTT %s:%d … ", MQTT_HOST, MQTT_PORT);
        if (mqtt.connect(DEVICE_SERIAL, MQTT_USER, MQTT_PASS)) {
            Serial.println("connected");
            mqtt.subscribe(topicConfig);
            mqtt.subscribe(topicOTA);
        } else {
            Serial.printf("failed rc=%d retry 5s\n", mqtt.state());
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    snprintf(topicTelemetry, 80, "devices/%s/telemetry", DEVICE_SERIAL);
    snprintf(topicConfig,    80, "devices/%s/config",    DEVICE_SERIAL);
    snprintf(topicOTA,       80, "devices/%s/ota",       DEVICE_SERIAL);
    connectWifi();
    connectMQTT();
}

void loop() {
    if (!mqtt.connected()) connectMQTT();
    mqtt.loop();

    static unsigned long last = 0;
    if (millis() - last >= 5000) {
        // Replace with real sensor reads (e.g. DHT22)
        float temp = 22.5f;
        float hum  = 60.0f;

        char payload[64];
        snprintf(payload, 64, "{\"temperature\":%.1f,\"humidity\":%.1f}", temp, hum);
        mqtt.publish(topicTelemetry, payload);
        Serial.printf("[EC] Published: %s\n", payload);
        last = millis();
    }
}
