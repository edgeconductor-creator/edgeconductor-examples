/**
 * EdgeConductor ESP32 Quickstart
 * ================================
 * Connects your ESP32 to EdgeConductor via MQTT over TLS.
 * Sends temperature + humidity every 30 seconds.
 * Receives OTA firmware updates automatically.
 *
 * Libraries needed (PlatformIO / Arduino):
 *   - PubSubClient
 *   - ArduinoJson
 *   - WiFiClientSecure (built-in)
 *
 * Fill in secrets.h with your credentials.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secrets.h"   // copy secrets.example.h → secrets.h and fill in

// ── MQTT topics ───────────────────────────────────────────────────────────────
#define TOPIC_TELEMETRY  "devices/" DEVICE_SERIAL "/telemetry"
#define TOPIC_CONFIG     "devices/" DEVICE_SERIAL "/config"
#define TOPIC_OTA        "devices/" DEVICE_SERIAL "/ota"

WiFiClientSecure tlsClient;
PubSubClient     mqtt(tlsClient);

// ── MQTT message handler ──────────────────────────────────────────────────────
void onMessage(char* topic, byte* payload, unsigned int len) {
    String t = String(topic);

    if (t.endsWith("/ota")) {
        // OTA firmware update URL received — implement your OTA handler here
        StaticJsonDocument<256> doc;
        deserializeJson(doc, payload, len);
        const char* url = doc["url"];
        Serial.printf("[OTA] Update available: %s\n", url ? url : "—");
        // Use ESP32 HTTPUpdate or your preferred OTA library
    }
}

// ── WiFi + MQTT connect ───────────────────────────────────────────────────────
void connectWifi() {
    Serial.printf("WiFi → %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println(" connected");
}

void connectMqtt() {
    tlsClient.setInsecure();   // use setCACert() for production cert pinning
    mqtt.setServer(MQTT_HOST, MQTT_PORT);
    mqtt.setCallback(onMessage);
    mqtt.setBufferSize(1024);

    while (!mqtt.connected()) {
        Serial.printf("MQTT → %s:%d … ", MQTT_HOST, MQTT_PORT);
        if (mqtt.connect(DEVICE_SERIAL, MQTT_USER, MQTT_PASS)) {
            Serial.println("connected");
            mqtt.subscribe(TOPIC_CONFIG);
            mqtt.subscribe(TOPIC_OTA);
        } else {
            Serial.printf("failed (rc=%d) retrying in 5s\n", mqtt.state());
            delay(5000);
        }
    }
}

// ── Telemetry ─────────────────────────────────────────────────────────────────
void sendTelemetry() {
    // Replace with real sensor reads (DHT22, SHT31, BME280, etc.)
    float temperature = 24.5 + (random(-20, 40) / 10.0);
    float humidity    = 58.0 + (random(-50, 100) / 10.0);

    StaticJsonDocument<128> doc;
    doc["temperature"] = temperature;
    doc["humidity"]    = humidity;

    char buf[128];
    serializeJson(doc, buf);
    mqtt.publish(TOPIC_TELEMETRY, buf);
    Serial.printf("[TELEMETRY] temp=%.1f hum=%.1f\n", temperature, humidity);
}

// ── Arduino setup / loop ──────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    connectWifi();
    connectMqtt();
}

unsigned long lastTelemetry = 0;

void loop() {
    if (!mqtt.connected()) connectMqtt();
    mqtt.loop();

    if (millis() - lastTelemetry > 30000) {   // every 30 seconds
        sendTelemetry();
        lastTelemetry = millis();
    }
}
