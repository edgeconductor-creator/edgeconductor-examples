# EdgeConductor Python SDK

`pip install edgeconductor` gives you a full Python client for device management, telemetry, automation rules, and fleet operations — works on Raspberry Pi, servers, and any Python 3.8+ environment.

## Install

```bash
pip install edgeconductor
```

## Quickstart

```python
from edgeconductor import Client

ec = Client(api_key="ec_live_xxxx")  # Dashboard → Settings → API Keys
```

---

## What you can do

### Device management

```python
# Register a device (idempotent — safe to call repeatedly)
device = ec.devices.register(
    serial_no="EC-FARM-001",
    product_type="climate_sensor",
)
print(device["mqtt_password"])  # store this in your firmware's flash

# List all devices in an org
devices = ec.devices.list(org_id="<org-id>")
for d in devices:
    print(d["serial_no"], d["status"])  # online / offline / never_seen

# Get device details + shadow
d = ec.devices.get("EC-FARM-001")
print(d["shadow_reported"])  # last telemetry values

# Push config to device via MQTT
ec.devices.push_config("EC-FARM-001", {"relay": True, "setpoint": 22})

# Reboot device remotely
ec.devices.reboot("EC-FARM-001")
```

### Telemetry — push + stream

```python
# Push telemetry (useful for testing, scripts, or Linux devices)
ec.telemetry.push("EC-FARM-001",
    temp=24.5, hum=60, co2=820, bat=4.1,
)

# Last 24 hours of history
rows = ec.telemetry.history("EC-FARM-001", hours=24)
for r in rows:
    print(r["timestamp"], r["temp"])

# Last 20 readings (newest first)
recent = ec.telemetry.recent("EC-FARM-001", limit=20)

# Live stream — callback fires every 5s, only when value changes
handle = ec.telemetry.stream(
    "EC-FARM-001",
    callback=lambda data: print("Live:", data),
    interval_ms=5000,
)
# stop streaming:
handle.stop()
```

### Rules + automation

```python
# Alert when CO₂ > 1000 ppm
rule = ec.rules.threshold(
    org_id="<org-id>",
    name="CO2 High Alert",
    field="co2",
    op=">",
    value=1000,
    action={"key": "relay", "value": True},
    webhook_url="https://hooks.example.com/alert",  # optional
)

# Schedule — turn relay off at 22:00 on weekdays
ec.rules.schedule(
    org_id="<org-id>",
    name="Night Off",
    time="22:00",
    days=["mon", "tue", "wed", "thu", "fri"],
    action={"key": "relay", "value": False},
)

# Enable / disable rules without deleting
ec.rules.disable(rule["id"])
ec.rules.enable(rule["id"])

# List and delete
rules = ec.rules.list("<org-id>")
ec.rules.delete(rules[0]["id"])
```

### OTA firmware

```python
# List firmware releases uploaded to the platform
releases = ec.firmware.list()
latest = releases[0]["id"]

# Push to a device
ec.firmware.push("EC-FARM-001", latest)
```

### Audit + notifications

```python
# Audit log — who did what, when
events = ec.audit.list(org_id="<org-id>", limit=50)
for e in events:
    print(e["action"], e["created_at"])

# In-app notifications (device offline, anomaly detected, etc.)
notifs = ec.notifications.list("<org-id>")
ec.notifications.mark_all_read("<org-id>")

# Anomaly alerts
anomalies = ec.anomalies.list("<org-id>", unacked=True)
ec.anomalies.acknowledge(anomalies[0]["id"])
```

### API key management

```python
# Generate a new key (shown once — store it immediately)
key = ec.api_keys.generate(org_id="<org-id>", name="CI pipeline")
print(key["key"])  # ec_live_xxxx

# List keys (prefix only — raw key never returned again)
keys = ec.api_keys.list("<org-id>")

# Revoke
ec.api_keys.revoke(keys[0]["id"])
```

---

## Run the quickstart

```bash
# Edit quickstart.py — fill in API_KEY and DEVICE_ID
python quickstart.py
```

Open your dashboard at [edgeconductor.com](https://edgeconductor.com) — live data will appear in under 5 seconds.

---

## Links

- [PyPI: edgeconductor](https://pypi.org/project/edgeconductor/)
- [SDK Reference — edgeconductor.com/developers](https://edgeconductor.com/developers)
- [ESP32 hardware example](../esp32/)
- [Raspberry Pi MQTT example](../raspberry-pi/)
