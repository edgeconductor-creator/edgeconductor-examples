#pragma once

// WiFi
#define WIFI_SSID   "your-wifi-name"
#define WIFI_PASS   "your-wifi-password"

// Device — must match what you registered in the dashboard
#define DEVICE_SERIAL  "EC-MY-DEVICE-001"

// MQTT — from Dashboard → Devices → [your device] → MQTT Credentials
#define MQTT_HOST  "services.edgeconductor.com"
#define MQTT_PORT  8883
#define MQTT_USER  DEVICE_SERIAL
#define MQTT_PASS  "your-per-device-password"
