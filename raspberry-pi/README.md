# Raspberry Pi / Linux — EdgeConductor

Connect any Raspberry Pi or Linux SBC (Jetson Nano, Orange Pi, Rock Pi, etc.) to EdgeConductor via MQTT TLS using Python.

## Works on

- Raspberry Pi (any model — Pi Zero W, Pi 3, Pi 4, Pi 5)
- Jetson Nano / Orin
- Orange Pi, Rock Pi, Banana Pi
- Any Linux machine with Python 3.8+

---

## Step 1 — Register your device

Dashboard → **Devices → Add Device** → select **Raspberry Pi / Linux** → enter a serial (e.g. `RPI-GATEWAY-01`) → click **Register**.

Click **↓ Download ZIP** — this gives you a pre-filled `device.py` with your credentials already in it.

---

## Step 2 — Install dependency

```bash
pip install paho-mqtt
# or
pip3 install paho-mqtt
```

---

## Step 3 — Run

If you used **Download ZIP**, just extract and run:
```bash
python device.py
```

If adding credentials manually, edit `device.py`:
```python
DEVICE_SERIAL = "RPI-GATEWAY-01"         # must match dashboard
MQTT_PASS     = "your-mqtt-password"     # from dashboard modal
```

Then:
```bash
python device.py
```

Expected output:
```
[EC] Connecting to services.edgeconductor.com:8883 ...
[EC] Connected — RPI-GATEWAY-01
[EC] Published: {'temperature': 23.1, 'humidity': 61.0}
[EC] Published: {'temperature': 23.2, 'humidity': 60.8}
```

Dashboard → Devices → your device goes **active** and data streams in live.

---

## What the script does

- Connects MQTT over TLS (port 8883) with auto-reconnect
- Publishes telemetry every 5 seconds to `devices/{serial}/telemetry`
- Subscribes to `devices/{serial}/config` for remote config/commands
- Subscribes to `devices/{serial}/ota` for OTA update notifications

---

## Add real sensor readings

Replace the dummy values in `device.py`:

```python
# Example: DHT22 via Adafruit library
import adafruit_dht, board
dht = adafruit_dht.DHT22(board.D4)

payload = {
    "temperature": dht.temperature,
    "humidity":    dht.humidity,
}
```

```python
# Example: CPU temperature (Pi built-in)
import subprocess
cpu_temp = float(subprocess.check_output(
    ["vcgencmd", "measure_temp"]
).decode().split("=")[1].replace("'C\n",""))

payload = {"cpu_temp": cpu_temp}
```

---

## Run as a service (auto-start on boot)

```bash
sudo nano /etc/systemd/system/ec-device.service
```

```ini
[Unit]
Description=EdgeConductor Device Agent
After=network-online.target
Wants=network-online.target

[Service]
ExecStart=/usr/bin/python3 /home/pi/edgeconductor-examples/raspberry-pi/device.py
Restart=always
RestartSec=10
User=pi

[Install]
WantedBy=multi-user.target
```

```bash
sudo systemctl enable ec-device
sudo systemctl start ec-device
sudo journalctl -u ec-device -f   # watch logs
```

---

## Links

- [Dashboard](https://ec-platform-ten.vercel.app)
- [Python SDK](../python/) — for more advanced control (rules, OTA, device management)
- [SDK Reference — edgeconductor.com/developers](https://edgeconductor.com/developers)
