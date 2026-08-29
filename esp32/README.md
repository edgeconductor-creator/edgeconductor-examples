# ESP32 Quickstart

Connect a real ESP32 to EdgeConductor via MQTT over TLS — live telemetry + OTA updates.

## Hardware

Any ESP32 board works — ESP32 DevKit, WROOM, WROVER, etc.

## Setup

### 1. Install PlatformIO
Install [PlatformIO](https://platformio.org/) (VS Code extension or CLI).

### 2. Clone this repo and open the `esp32/` folder

### 3. Create your secrets file
```bash
cp secrets.example.h secrets.h
```
Edit `secrets.h` with your WiFi credentials and MQTT password from the dashboard.

### 4. Register your device
In your dashboard → **Devices → Add Device** → use the same serial as `DEVICE_SERIAL` in `secrets.h`.

### 5. Flash
```bash
pio run --target upload
pio device monitor
```

## What you'll see

```
WiFi → MyNetwork ... connected
MQTT → services.edgeconductor.com:8883 … connected
[TELEMETRY] temp=24.5 hum=58.2
[TELEMETRY] temp=24.8 hum=57.9
```

Open your dashboard — the device goes online and telemetry streams in live.

## Add a real sensor

Replace the random values in `sendTelemetry()` with your sensor library:

```cpp
// DHT22
#include <DHT.h>
DHT dht(4, DHT22);
float temperature = dht.readTemperature();
float humidity    = dht.readHumidity();
```

## OTA Updates

Push a firmware binary from **Dashboard → Devices → [device] → OTA Update**.  
The ESP32 receives the URL via MQTT and downloads + flashes automatically.
