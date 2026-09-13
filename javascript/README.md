# EdgeConductor JS SDK

`npm install ec-sdk` gives you a full JavaScript/TypeScript client for device management, telemetry, rules, and fleet operations — works in Node.js, Raspberry Pi (Node), or any server-side JS environment.

## Install

```bash
npm install ec-sdk
```

## Quickstart

```js
const { EdgeConductor } = require('ec-sdk');

const ec = new EdgeConductor({ apiKey: 'ec_live_xxxx' });
// → connects to services.edgeconductor.com
```

---

## What you can do

### Device management

```js
// Register a device (idempotent — safe to call repeatedly)
const device = await ec.devices.register({
  serial_no: 'EC-FARM-001',
  product_type: 'climate_sensor',
});
console.log(device.mqtt_password); // store in firmware flash

// List all devices in an org
const devices = await ec.devices.list({ orgId: '<org-id>' });
devices.forEach(d => console.log(d.serial_no, d.status));

// Get device details + shadow (last reported telemetry)
const d = await ec.devices.get('EC-FARM-001');
console.log(d.shadow_reported);

// Push config to device via MQTT
await ec.devices.pushConfig('EC-FARM-001', { relay: true, setpoint: 22 });

// Reboot device remotely
await ec.devices.reboot('EC-FARM-001');
```

### Telemetry — push + stream

```js
// Push telemetry (useful for testing, scripts, or Linux devices)
await ec.telemetry.push('EC-FARM-001', {
  temp: 24.5, hum: 60, co2: 820, bat: 4.1,
});

// Last 24 hours of history
const rows = await ec.telemetry.history('EC-FARM-001', { hours: 24 });
rows.forEach(r => console.log(r.timestamp, r.temp));

// Last 20 readings (newest first)
const recent = await ec.telemetry.recent('EC-FARM-001', 20);

// Live stream — callback fires every 5s, only when value changes
const handle = ec.telemetry.stream(
  'EC-FARM-001',
  (data) => console.log('Live:', data),
  { intervalMs: 5000 },
);
// stop streaming:
handle.stop();
```

### Rules + automation

```js
// Alert when CO₂ > 1000 ppm
const rule = await ec.rules.threshold('<org-id>', {
  name: 'CO2 High Alert',
  field: 'co2',
  op: '>',
  value: 1000,
  action: { key: 'relay', value: true },
  webhookUrl: 'https://hooks.example.com/alert', // optional
});

// Schedule — turn relay off at 22:00 on weekdays
await ec.rules.schedule('<org-id>', {
  name: 'Night Off',
  time: '22:00',
  days: ['mon', 'tue', 'wed', 'thu', 'fri'],
  action: { key: 'relay', value: false },
});

// Enable / disable rules without deleting
await ec.rules.disable(rule.id);
await ec.rules.enable(rule.id);
```

### OTA firmware

```js
// List firmware releases
const releases = await ec.firmware.list();
const latest = releases[0].id;

// Push to a device
await ec.firmware.push('EC-FARM-001', latest);
```

### API key management

```js
// Generate a new key (shown once — store it immediately)
const { key } = await ec.apiKeys.generate('<org-id>', 'CI pipeline');
console.log(key); // ec_live_xxxx

// List keys (prefix only)
const keys = await ec.apiKeys.list('<org-id>');

// Revoke
await ec.apiKeys.revoke(keys[0].id);
```

---

## Run the quickstart

```bash
# Edit quickstart.js — fill in API_KEY and DEVICE_ID
node quickstart.js
```

Open your dashboard at [edgeconductor.com](https://edgeconductor.com) — live data will appear in under 5 seconds.

---

## Links

- [npm: ec-sdk](https://www.npmjs.com/package/ec-sdk)
- [SDK Reference — edgeconductor.com/developers](https://edgeconductor.com/developers)
- [ESP32 hardware example](../esp32/)
- [Python SDK example](../python/)
