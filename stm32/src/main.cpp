/**
 * EdgeConductor — STM32 + ESP8266 AT WiFi
 * =========================================
 * STM32 handles application logic.
 * ESP8266 (ESP-AT v2.4+) handles WiFi + MQTT TLS.
 *
 * Wiring:
 *   STM32 PA2 (TX2) → ESP8266 RX
 *   STM32 PA3 (RX2) → ESP8266 TX
 *   ESP8266 CH_EN   → 3.3V
 *
 * Setup:
 *   1. Copy secrets.example.h → secrets.h, fill in credentials
 *   2. pio run -e bluepill --target upload
 */

#include <Arduino.h>
#include "secrets.h"

HardwareSerial espSerial(PA3, PA2);  // RX, TX
static char atBuf[512];

static bool atSend(const char* cmd, const char* expect, uint32_t ms = 5000) {
    while (espSerial.available()) espSerial.read();
    espSerial.println(cmd);
    Serial.printf(">> %s\n", cmd);
    uint32_t t = millis(); uint16_t i = 0;
    while (millis() - t < ms) {
        while (espSerial.available() && i < sizeof(atBuf) - 1)
            atBuf[i++] = espSerial.read();
        atBuf[i] = '\0';
        if (strstr(atBuf, expect)) { Serial.printf("<< OK\n"); return true; }
        if (strstr(atBuf, "ERROR")) return false;
    }
    return false;
}

void setup() {
    Serial.begin(115200);
    espSerial.begin(115200);
    delay(2000);

    atSend("AT",        "OK",         3000);
    atSend("AT+RST",    "ready",      5000); delay(1000);
    atSend("AT+CWMODE=1", "OK",       3000);

    char buf[256];
    snprintf(buf, sizeof(buf), "AT+CWJAP=\"%s\",\"%s\"", WIFI_SSID, WIFI_PASS);
    atSend(buf, "WIFI GOT IP", 15000);

    // MQTT over TLS — mode 4 = TLS without client cert
    snprintf(buf, sizeof(buf),
        "AT+MQTTUSERCFG=0,4,\"%s\",\"%s\",\"%s\",0,0,\"\"",
        DEVICE_SERIAL, MQTT_USER, MQTT_PASS);
    atSend(buf, "OK", 5000);

    snprintf(buf, sizeof(buf), "AT+MQTTCONN=0,\"%s\",%d,1", MQTT_HOST, MQTT_PORT);
    atSend(buf, "MQTTCONNECTED", 10000);

    snprintf(buf, sizeof(buf), "AT+MQTTSUB=0,\"devices/%s/config\",1", DEVICE_SERIAL);
    atSend(buf, "OK", 5000);

    Serial.println("[EC] Ready");
}

void loop() {
    static uint32_t last = 0;
    if (millis() - last >= 5000) {
        // Replace with real sensor reads
        float temp = 22.5f;
        float hum  = 60.0f;

        char buf[256], payload[64];
        snprintf(payload, sizeof(payload),
                 "{\"temperature\":%.1f,\"humidity\":%.1f}", temp, hum);
        snprintf(buf, sizeof(buf),
                 "AT+MQTTPUB=0,\"devices/%s/telemetry\",\"%s\",1,0",
                 DEVICE_SERIAL, payload);
        atSend(buf, "OK", 5000);
        Serial.printf("[EC] Published: %s\n", payload);
        last = millis();
    }
}
