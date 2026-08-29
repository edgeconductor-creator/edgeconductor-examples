# ESP32 Quickstart

Connect a real ESP32 to EdgeConductor via MQTT over TLS — live telemetry + OTA updates.

## Hardware

Any ESP32 board works — ESP32 DevKit, WROOM, WROVER, etc.

## Step 1 — Register your device

Dashboard → **Devices → Add Device** → enter a serial (e.g. `EC-MY-DEVICE-001`) → click **Register**.

Copy the MQTT password from the modal or click **↓ Download secrets.h** — save it, it's shown only once.

---

## Option A — Arduino IDE (easier for beginners)

### 1. Install Arduino IDE
Download from [arduino.cc/en/software](https://www.arduino.cc/en/software)

### 2. Add ESP32 board support
File → Preferences → Additional Boards Manager URLs, add:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
Then Tools → Board → Boards Manager → search **esp32** → Install.

### 3. Install libraries
Tools → Manage Libraries → search and install:
- **PubSubClient** (by Nick O'Leary)
- **ArduinoJson** (by Benoit Blanchon)

### 4. Open the sketch
File → Open → `esp32/arduino/EdgeConductorBasic/EdgeConductorBasic.ino`

### 5. Add your credentials
In the same folder, copy `secrets_example.h` → `secrets.h`.  
Paste the values from the dashboard (or use the downloaded file directly).

### 6. Select board & port
Tools → Board → **ESP32 Dev Module**  
Tools → Port → your ESP32 COM port

### 7. Upload
Click **→ Upload**, then open **Serial Monitor** at 115200 baud.

```
WiFi → MyNetwork ... connected
MQTT → services.edgeconductor.com:8883 … connected
[TELEMETRY] temp=24.5 hum=58.2
```

---

## Option B — PlatformIO (recommended for teams)

### 1. Install PlatformIO
Install the [PlatformIO VS Code extension](https://platformio.org/install/ide?install=vscode).

### 2. Open the project
Open the `esp32/` folder in VS Code. PlatformIO auto-detects `platformio.ini`.

### 3. Add your credentials
Copy `secrets.example.h` → `secrets.h` and fill in your values (or use the downloaded file).

### 4. Flash
```bash
pio run --target upload
pio device monitor
```

---

## What you'll see

```
WiFi → MyNetwork ... connected
MQTT → services.edgeconductor.com:8883 … connected
[TELEMETRY] temp=24.5 hum=58.2
[TELEMETRY] temp=24.8 hum=57.9
```

Dashboard → Devices → your device goes **active** and telemetry streams in live.

## Add a real sensor

Replace the random values in `sendTelemetry()`:

```cpp
// DHT22 example
#include <DHT.h>
DHT dht(4, DHT22);
float temperature = dht.readTemperature();
float humidity    = dht.readHumidity();
```

## OTA Updates

Push a firmware binary from **Dashboard → Devices → [device] → OTA Update**.  
The ESP32 receives the URL via MQTT and downloads + flashes automatically.
