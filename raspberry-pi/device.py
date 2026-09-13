"""
EdgeConductor — Raspberry Pi / Linux Device
============================================
Works on: Raspberry Pi, Jetson Nano, Orange Pi, any Linux with Python 3.8+

Install:  pip install paho-mqtt
Run:      python device.py

Get credentials: Dashboard → Add Device → Raspberry Pi / Linux → Download Firmware ZIP
"""

import time, json, random, ssl
import paho.mqtt.client as mqtt

# ── Credentials — fill in from dashboard ─────────────────────────────────────
DEVICE_SERIAL = "YOUR-SERIAL"          # e.g. RPI-GATEWAY-01
MQTT_PASS     = "your-mqtt-password"   # from Dashboard → Add Device credentials
# ─────────────────────────────────────────────────────────────────────────────

MQTT_HOST = "services.edgeconductor.com"
MQTT_PORT = 8883

TOPIC_TELEMETRY = f"devices/{DEVICE_SERIAL}/telemetry"
TOPIC_CONFIG    = f"devices/{DEVICE_SERIAL}/config"
TOPIC_OTA       = f"devices/{DEVICE_SERIAL}/ota"


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print(f"[EC] Connected — {DEVICE_SERIAL}")
        client.subscribe(TOPIC_CONFIG)
        client.subscribe(TOPIC_OTA)
    else:
        print(f"[EC] Connection failed rc={rc}")


def on_message(client, userdata, msg):
    data = json.loads(msg.payload)
    if msg.topic == TOPIC_CONFIG:
        print(f"[EC] Config update: {data}")
        # TODO: act on config (relay, setpoint, etc.)
    elif msg.topic == TOPIC_OTA:
        print(f"[EC] OTA update available: {data.get('url')}")


client = mqtt.Client(client_id=DEVICE_SERIAL, clean_session=True)
client.username_pw_set(DEVICE_SERIAL, MQTT_PASS)
client.tls_set(cert_reqs=ssl.CERT_REQUIRED, tls_version=ssl.PROTOCOL_TLSv1_2)
client.on_connect = on_connect
client.on_message = on_message
client.reconnect_delay_set(min_delay=5, max_delay=60)

print(f"[EC] Connecting to {MQTT_HOST}:{MQTT_PORT} ...")
client.connect(MQTT_HOST, MQTT_PORT, keepalive=60)
client.loop_start()
time.sleep(2)

try:
    while True:
        # ── Replace with your real sensor readings ────────────────────────────
        payload = {
            "temperature": round(22.5 + random.uniform(-2, 2), 1),
            "humidity":    round(60.0 + random.uniform(-5, 5),  1),
        }
        # ─────────────────────────────────────────────────────────────────────
        client.publish(TOPIC_TELEMETRY, json.dumps(payload), qos=1)
        print(f"[EC] Published: {payload}")
        time.sleep(5)

except KeyboardInterrupt:
    print("[EC] Stopped.")
    client.loop_stop()
    client.disconnect()
