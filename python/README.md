# Python Quickstart

Send live telemetry from any Python script — Raspberry Pi, server, or laptop.

## Setup

```bash
pip install edgeconductor
```

## Run

1. Get your API key from **Dashboard → Settings → API Keys**
2. Edit `quickstart.py` — fill in `API_KEY` and `DEVICE_ID`
3. Run:

```bash
python quickstart.py
```

Open your dashboard — you'll see live temperature, humidity, and CO₂ data streaming in.

## What it does

- Registers the device (creates it if it doesn't exist)
- Pushes telemetry every 5 seconds
- Dashboard updates in real time

## Next steps

- Set up rules: **Dashboard → Automation** → alert when temperature > 30°C
- View history charts in the device detail page
- Try the [ESP32 example](../esp32/) to connect real hardware
