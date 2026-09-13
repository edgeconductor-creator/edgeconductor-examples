# EdgeConductor Examples

Connect your devices to a live IoT dashboard in minutes — no backend to build, no infra to manage.

[EdgeConductor](https://edgeconductor.com) gives you device registry, live telemetry, OTA firmware updates, rules engine, and white-label dashboards out of the box.

---

## Quick Start

1. **Create a free account** → [edgeconductor.com/register](https://edgeconductor.com/register)
2. **Add a device** in the dashboard — copy your `device_id` and `api_key`
3. **Run an example** below

---

## Examples

| Example | Hardware | Description |
|---------|----------|-------------|
| [esp32/](esp32/) | ESP32 (PlatformIO) | MQTT TLS — telemetry + config + OTA |
| [esp32/arduino/](esp32/arduino/) | ESP32 (Arduino IDE) | Same, .ino format for Arduino IDE |
| [esp8266/](esp8266/) | ESP8266 / NodeMCU | WiFi MQTT TLS via PlatformIO |
| [raspberry-pi/](raspberry-pi/) | RPi / Linux | Python paho-mqtt — any Linux SBC |
| [stm32/](stm32/) | STM32 + ESP8266 AT | AT command WiFi coprocessor |
| [python/](python/) | Any device | Python SDK quickstart |
| [javascript/](javascript/) | Node.js / RPi | Node.js SDK quickstart |

---

## How It Works

```
Your Device  ──MQTT──▶  EdgeConductor Cloud  ──▶  Dashboard
     │                         │
     │◀──── OTA Firmware ──────┤
     │◀──── Commands    ───────┘
```

- **Registry** — device metadata, status, config
- **Telemetry** — real-time sensor data streaming
- **OTA** — push firmware updates to any device
- **Rules** — trigger alerts, automations on telemetry values
- **Dashboard** — white-label, multi-tenant, your branding

---

## Platform URLs

| Service | URL |
|---------|-----|
| MQTT Broker | `services.edgeconductor.com:8883` (TLS) |
| REST API | `https://services.edgeconductor.com` |
| Dashboard | `https://edgeconductor.com` |

---

## Links

- Website: [edgeconductor.com](https://edgeconductor.com)
- Docs: [edgeconductor.com/developers](https://edgeconductor.com/developers)
- npm: [`ec-sdk`](https://www.npmjs.com/package/ec-sdk)
- PyPI: [`edgeconductor`](https://pypi.org/project/edgeconductor/)

---

## License

MIT
