# ESP8266 / NodeMCU — EdgeConductor

Connect an ESP8266 (NodeMCU, Wemos D1 Mini, etc.) to EdgeConductor via MQTT over TLS.

## Hardware

Any ESP8266 board works — NodeMCU v2, Wemos D1 Mini, ESP-12F, etc.

## Step 1 — Register your device

Dashboard → **Devices → Add Device** → select **ESP8266** → enter a serial (e.g. `EC-MY-001`) → click **Register**.

Copy the **MQTT password** from the modal or click **↓ Download ZIP** — save it, shown only once.

---

## Step 2 — Setup PlatformIO

Install the [PlatformIO VS Code extension](https://platformio.org/install/ide?install=vscode).

Open the `esp8266/` folder in VS Code. PlatformIO auto-detects `platformio.ini`.

Board supported out of the box: **NodeMCU v2** (`nodemcuv2`). To use a different board, change the `board` line in `platformio.ini`.

---

## Step 3 — Add credentials

Copy `secrets.example.h` → `secrets.h` and fill in your values:

```h
#define WIFI_SSID   "YourNetwork"
#define WIFI_PASS   "YourPassword"
#define DEVICE_SERIAL  "EC-MY-001"          // must match dashboard
#define MQTT_PASS      "your-mqtt-password" // from dashboard modal
```

Or use the **Download ZIP** button in the dashboard — it creates a pre-filled `secrets.h`.

---

## Step 4 — Flash

```bash
pio run --target upload
pio device monitor    # 115200 baud
```

Expected serial output:
```
WiFi → YourNetwork ... connected
MQTT → services.edgeconductor.com:8883 ... connected
[TELEMETRY] {"temperature":24.5,"humidity":58.1}
[TELEMETRY] {"temperature":24.7,"humidity":57.9}
```

Dashboard → Devices → your device goes **active** and data streams in live.

---

## What the firmware does

- Connects WiFi, then MQTT over TLS (port 8883)
- Publishes telemetry every 5 seconds to `devices/{serial}/telemetry`
- Subscribes to `devices/{serial}/config` for remote config updates
- Auto-reconnects on WiFi or MQTT drop

## Add a real sensor

Replace the random values in `loop()`:

```cpp
#include <DHT.h>
DHT dht(D4, DHT22);  // data on D4

float temp = dht.readTemperature();
float hum  = dht.readHumidity();
```

---

## Wiring (NodeMCU v2)

| Function | Pin |
|----------|-----|
| DHT22 data | D4 (GPIO2) |
| DHT22 VCC | 3.3V |
| DHT22 GND | GND |

---

## Links

- [Dashboard](https://ec-platform-ten.vercel.app)
- [SDK Reference — edgeconductor.com/developers](https://edgeconductor.com/developers)
- [ESP32 example](../esp32/) — if you need more RAM / processing power
