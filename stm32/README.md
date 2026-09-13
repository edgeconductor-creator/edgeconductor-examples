# STM32 + ESP8266 AT Coprocessor — EdgeConductor

STM32 handles application logic. ESP8266 (running ESP-AT firmware) handles WiFi + MQTT TLS as a coprocessor — no TLS library needed on the STM32.

## How it works

```
STM32 (main CPU)  ──UART──▶  ESP8266 (WiFi + MQTT)  ──TLS──▶  services.edgeconductor.com
   PA2 TX2                       RX
   PA3 RX2                       TX
```

The STM32 sends AT commands over UART2 (PA2/PA3). The ESP8266 handles all WiFi and MQTT TLS internally.

## Hardware

| Board | Config |
|-------|--------|
| STM32F103C8 (Blue Pill) | `env:bluepill` |
| STM32F446RE (Nucleo) | `env:nucleo_f446re` |

Any ESP8266 module works — ESP-01, ESP-12F, Wemos D1 Mini (as AT coprocessor).

**Wiring:**

| STM32 | ESP8266 | Notes |
|-------|---------|-------|
| PA2 (TX2) | RX | UART data STM32→ESP |
| PA3 (RX2) | TX | UART data ESP→STM32 |
| 3.3V | CH_EN | Must be HIGH to enable ESP |
| 3.3V | VCC | ESP power |
| GND | GND | Common ground |

> The ESP8266 must be flashed with **ESP-AT v2.4+** firmware. Check: send `AT+GMR` — should return `AT version:2.4.x`.

---

## Step 1 — Register your device

Dashboard → **Devices → Add Device** → select **STM32** → enter a serial → click **Register**.

Copy the **MQTT password** or click **↓ Download ZIP**.

---

## Step 2 — Setup PlatformIO

Install the [PlatformIO VS Code extension](https://platformio.org/install/ide?install=vscode).

Open the `stm32/` folder in VS Code. PlatformIO auto-detects `platformio.ini`.

---

## Step 3 — Add credentials

Copy `secrets.example.h` → `secrets.h` and fill in:

```h
#define WIFI_SSID   "YourNetwork"
#define WIFI_PASS   "YourPassword"
#define DEVICE_SERIAL  "EC-STM32-001"       // must match dashboard
#define MQTT_PASS      "your-mqtt-password" // from dashboard modal
```

Or use the **Download ZIP** button — pre-fills `secrets.h`.

---

## Step 4 — Flash

```bash
# Blue Pill (requires ST-Link)
pio run -e bluepill --target upload

# Nucleo
pio run -e nucleo_f446re --target upload

pio device monitor    # 115200 baud
```

Expected serial output:
```
>> AT
<< OK
>> AT+CWJAP="YourNetwork","YourPassword"
<< WIFI GOT IP
>> AT+MQTTCONN=0,"services.edgeconductor.com",8883,1
<< MQTTCONNECTED
[EC] Ready
[EC] Published: {"temperature":22.5,"humidity":60.0}
```

---

## AT Command flow

The firmware runs this sequence on boot:

1. `AT` — check ESP alive
2. `AT+RST` — reset ESP
3. `AT+CWMODE=1` — station mode
4. `AT+CWJAP` — join WiFi
5. `AT+MQTTUSERCFG` — set MQTT credentials (mode 4 = TLS without client cert)
6. `AT+MQTTCONN` — connect to broker
7. `AT+MQTTSUB` — subscribe to config topic
8. Loop: `AT+MQTTPUB` — publish telemetry every 5s

## Add real sensors

Replace the dummy values in `loop()` in `src/main.cpp`:

```cpp
// Example: read ADC for a temperature sensor
float temp = analogRead(PA0) * 0.1f;  // calibrate per sensor
float hum  = 60.0f;                   // read from your sensor
```

---

## Links

- [Dashboard](https://ec-platform-ten.vercel.app)
- [SDK Reference — edgeconductor.com/developers](https://edgeconductor.com/developers)
- [ESP8266 standalone example](../esp8266/) — if you don't need STM32
