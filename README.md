# EdgeConductor Examples

Connect your devices to a live IoT dashboard in minutes — no backend to build, no infra to manage.

[EdgeConductor](https://edgeconductor.com) gives you device registry, live telemetry, OTA firmware updates, rules engine, and white-label dashboards out of the box.

---

## Quick Start

1. **Create a free account** → [edgeconductor.com/register](https://ec-platform-ten.vercel.app/register)
2. **Add a device** in the dashboard — copy your `device_id` and `api_key`
3. **Run an example** below

---

## Examples

| Example | Description |
|---------|-------------|
| [esp32/](esp32/) | Connect an ESP32 via MQTT — send telemetry + receive OTA updates |
| [python/](python/) | Python SDK — register device, send telemetry, listen for commands |
| [javascript/](javascript/) | Node.js SDK — same flow for server-side or Raspberry Pi use |
| [postman/](postman/) | REST API collection — explore every endpoint manually |

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
| Dashboard | `https://ec-platform-ten.vercel.app` |

---

## Links

- Website: [edgeconductor.com](https://edgeconductor.com)
- Docs: [edgeconductor.com/developers](https://edgeconductor.com/developers)
- npm: [`ec-sdk`](https://www.npmjs.com/package/ec-sdk)
- PyPI: [`edgeconductor`](https://pypi.org/project/edgeconductor/)

---

## License

MIT
