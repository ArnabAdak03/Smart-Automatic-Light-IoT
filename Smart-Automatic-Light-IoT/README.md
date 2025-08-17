# 💡 Smart Automatic Light Using IoT (ESP8266)

An IoT-based smart lighting system that **turns lights ON only when motion is detected in low-light conditions**.
Built with **NodeMCU (ESP8266)**, **PIR sensor**, **LDR (photoresistor)**, and a **relay module**. Optional
cloud control/monitoring via **Blynk**.

## ✨ Features
- Motion-activated lighting using PIR
- Ambient light sensing (LDR on A0) to avoid turning on during daylight
- Auto-off timer after no motion
- Configurable thresholds (via code; optional Blynk controls)
- Works offline; optional Blynk cloud for remote control

## 🧰 Hardware
- NodeMCU v2/v3 (ESP8266)
- PIR sensor (HC-SR501 or similar)
- LDR + 10kΩ resistor (voltage divider to A0)
- 1-Channel relay module (5V with optocoupler recommended)
- Power supply (5V/2A) and wiring

## 🔌 Wiring (Default)
- **PIR OUT → D5 (GPIO14)**
- **Relay IN → D1 (GPIO5)**
- **LDR divider → A0** (LDR to 3.3V, 10kΩ to GND, midpoint to A0)
- Common grounds: PIR GND, LDR GND, Relay GND, NodeMCU GND
- Relay **COM** to AC line; **NO** to lamp line (⚠️ High voltage — follow safety rules)

> You can change pins in `firmware/SmartAutoLight.ino`

## 🛠️ Firmware Setup
1. Install **Arduino IDE** + **ESP8266 Boards** (Boards Manager).
2. Install libraries (if using Blynk):
   - `Blynk` (legacy or IoT) and `BlynkSimpleEsp8266.h`
3. Open `firmware/SmartAutoLight.ino` and set:
   - `LDR_THRESHOLD`, `MOTION_HOLD_MS`, `RELAY_ACTIVE_LOW`
   - If enabling Blynk: set `WIFI_SSID`, `WIFI_PASS`, and `BLYNK_AUTH`
4. Select board **NodeMCU 1.0 (ESP-12E Module)** and flash.

## 🌐 Optional: Blynk (Cloud)
- Set `#define BLYNK_ENABLED 1` in the sketch.
- Fill WiFi + Blynk auth token.
- App widgets:
  - V0: Manual Relay Switch
  - V1: Motion indicator
  - V2: LDR value (live)
  - V3: LDR threshold (slider)
  - V4: Mode (Auto/Manual)

## ⚙️ Tuning
- Start with `LDR_THRESHOLD = 600` (0–1023). Increase to require darker room.
- `MOTION_HOLD_MS` controls how long the light stays on after last motion (default 15s).
- If your relay board is **active LOW**, keep `RELAY_ACTIVE_LOW = true`.

## 🧪 Test Plan
1. Cover LDR → simulate darkness.
2. Wave in front of PIR → lamp turns ON.
3. Stay still → lamp turns OFF after timer.
4. Brighten room → motion shouldn’t turn lamp ON.

## 🧯 Safety
- **Mains voltage is dangerous.** Use certified enclosures, proper insulation, and fuses.
- Prefer switching **low-voltage LED** or use an **SSR** (solid state relay).

## 📁 Repo Structure
```
Smart-Automatic-Light-IoT/
├─ firmware/
│  └─ SmartAutoLight.ino
├─ hardware/
│  └─ wiring.md
├─ docs/
│  └─ images/ (place screenshots/photos here)
├─ .gitignore
├─ LICENSE
└─ README.md
```

## 📜 License
MIT — see `LICENSE`.

---
Generated on 2025-08-17.


---

## 🧩 PlatformIO (Alternative to Arduino IDE)
You can also build/flash using VS Code + PlatformIO:
```bash
# Install PlatformIO (once)
pip install platformio

# Build
pio run

# Upload (connect NodeMCU via USB)
pio run -t upload

# Open serial monitor
pio device monitor -b 115200
```
PlatformIO environment is pre-configured in `platformio.ini`.

## 🗺️ Schematic
Quick view: `docs/wiring-diagram.png`  
Printable PDF: `docs/wiring-diagram.pdf`

## 📄 Sample Log
See `docs/test-log.txt` for an example serial session.

## 🧪 Simulation / Future Work
- Add a full Proteus/Fritzing schematic with part numbers.
- Replace relay with SSR for silent operation.
- Add NTP time window (only active at night).
- Add Blynk dashboard JSON export.
