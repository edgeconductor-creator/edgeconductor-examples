#pragma once

// WiFi
#define WIFI_SSID   "your-wifi-name"
#define WIFI_PASS   "your-wifi-password"

// Device — register in Dashboard → Devices → Add Device
// Use the exact serial number you entered there.
#define DEVICE_SERIAL  "EC-MY-DEVICE-001"

// MQTT — from Dashboard → Devices → [your device] → MQTT Credentials
#define MQTT_HOST  "services.edgeconductor.com"
#define MQTT_PORT  8883
#define MQTT_USER  DEVICE_SERIAL            // username = serial number
#define MQTT_PASS  "your-per-device-password"  // shown once at registration
