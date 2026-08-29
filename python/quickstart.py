"""
EdgeConductor Python Quickstart
================================
1. pip install edgeconductor
2. Fill in your API key and device serial below
3. python quickstart.py

Get your API key: Dashboard → Settings → API Keys
"""

from edgeconductor import Client
import time
import random

# ── Config ────────────────────────────────────────────────────────────────────
API_KEY     = "ec_live_YOUR_API_KEY"   # from Dashboard → Settings → API Keys
DEVICE_ID   = "MY-DEVICE-001"         # unique serial for your device
BASE_URL    = "https://services.edgeconductor.com/registry"

ec = Client(api_key=API_KEY, base_url=BASE_URL)

# ── Step 1: Register device (safe to call multiple times) ─────────────────────
print("Registering device...")
device = ec.devices.register(
    serial_no=DEVICE_ID,
    product_type="EC-CLIMATE-V1",   # or EC-TRACKER-V1
)
print(f"Device: {device['id']} — {device.get('status', 'registered')}")

# ── Step 2: Push telemetry in a loop ─────────────────────────────────────────
print("\nSending telemetry every 5 seconds. Open your dashboard to see live data.")
print("Press Ctrl+C to stop.\n")

while True:
    payload = {
        "temperature": round(22 + random.uniform(-2, 4), 1),
        "humidity":    round(55 + random.uniform(-5, 10), 1),
        "co2":         int(800 + random.uniform(-50, 200)),
    }
    ec.telemetry.push(DEVICE_ID, **payload)
    print(f"Sent: {payload}")
    time.sleep(5)
