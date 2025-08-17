# Wiring & Notes

Default pins (change in the sketch if needed):
- PIR OUT → D5 (GPIO14)
- Relay IN → D1 (GPIO5)
- LDR divider → A0 (LDR to 3.3V, 10kΩ to GND, midpoint to A0)

**LDR Divider:** Use 10kΩ as a starting point. Adjust for your lighting conditions.

**Relay Polarity:** Many relay modules are **active LOW**. If your relay turns ON at boot,
keep `RELAY_ACTIVE_LOW = true` (default) in the code.

**Power:** Use a stable 5V/2A supply. Do NOT drive relay coil from 3.3V.
