/**
 * EdgeConductor Node.js Quickstart
 * ==================================
 * 1. npm install ec-sdk
 * 2. Fill in your API key and device serial below
 * 3. node quickstart.js
 *
 * Get your API key: Dashboard → Settings → API Keys
 */

const { EdgeConductor } = require('ec-sdk');

// ── Config ────────────────────────────────────────────────────────────────────
const API_KEY   = 'ec_live_YOUR_API_KEY';  // from Dashboard → Settings → API Keys
const DEVICE_ID = 'MY-DEVICE-001';         // unique serial for your device
const BASE_URL  = 'https://services.edgeconductor.com/registry';

const ec = new EdgeConductor({ apiKey: API_KEY, baseUrl: BASE_URL });

async function main() {
  // Step 1: Register device
  console.log('Registering device...');
  const device = await ec.devices.register({
    serial:      DEVICE_ID,
    productType: 'EC-CLIMATE-V1',   // or EC-TRACKER-V1
  });
  console.log(`Device: ${device.id} — ${device.status || 'registered'}`);

  // Step 2: Push telemetry every 5 seconds
  console.log('\nSending telemetry every 5 seconds. Open your dashboard to see live data.');
  console.log('Press Ctrl+C to stop.\n');

  setInterval(async () => {
    const payload = {
      temperature: +(22 + (Math.random() * 6 - 2)).toFixed(1),
      humidity:    +(55 + (Math.random() * 15 - 5)).toFixed(1),
      co2:         Math.floor(800 + Math.random() * 250),
    };
    await ec.telemetry.push(DEVICE_ID, payload);
    console.log('Sent:', payload);
  }, 5000);
}

main().catch(console.error);
