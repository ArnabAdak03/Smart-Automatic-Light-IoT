// Smart Automatic Light Using IoT (ESP8266)
// NodeMCU + PIR + LDR + Relay
// Optional Blynk cloud control.
//
// Select board: NodeMCU 1.0 (ESP-12E Module)

// ------------- User Settings -------------
#define PIR_PIN           D5        // PIR digital output
#define RELAY_PIN         D1        // Relay input pin
#define LDR_PIN           A0        // Analog input (0..1023)

#define RELAY_ACTIVE_LOW  true      // Most relay boards are active LOW
#define LDR_THRESHOLD     600       // 0..1023 (higher = requires darker room)
#define MOTION_HOLD_MS    15000     // Keep light on this long after last motion
#define LDR_SMOOTH_ALPHA  0.2       // 0..1, EMA smoothing for LDR

#define BLYNK_ENABLED     0         // Set to 1 to enable Blynk features
// ----------------------------------------

#if BLYNK_ENABLED
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
  char WIFI_SSID[] = "YOUR_WIFI_SSID";
  char WIFI_PASS[] = "YOUR_WIFI_PASSWORD";
  char BLYNK_AUTH[] = "YOUR_BLYNK_AUTH_TOKEN";
  #define VPIN_RELAY   V0
  #define VPIN_MOTION  V1
  #define VPIN_LDR     V2
  #define VPIN_THRESH  V3
  #define VPIN_MODE    V4  // 0=Auto, 1=Manual
#endif

unsigned long lastMotionMs = 0;
bool relayOn = false;
bool manualMode = false;   // Blynk can set this to true for manual control
float ldrEma = 0;
int ldrThreshold = LDR_THRESHOLD;

void setRelay(bool on) {
  relayOn = on;
  #if RELAY_ACTIVE_LOW
    digitalWrite(RELAY_PIN, on ? LOW : HIGH);
  #else
    digitalWrite(RELAY_PIN, on ? HIGH : LOW);
  #endif
  #if BLYNK_ENABLED
    Blynk.virtualWrite(VPIN_RELAY, relayOn);
  #endif
}

void setupPins() {
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  // Ensure relay OFF at boot
  #if RELAY_ACTIVE_LOW
    digitalWrite(RELAY_PIN, HIGH);
  #else
    digitalWrite(RELAY_PIN, LOW);
  #endif
}

#if BLYNK_ENABLED
BLYNK_WRITE(VPIN_RELAY) {
  if (!manualMode) return;
  int v = param.asInt();
  setRelay(v != 0);
}

BLYNK_WRITE(VPIN_THRESH) {
  ldrThreshold = constrain(param.asInt(), 0, 1023);
}

BLYNK_WRITE(VPIN_MODE) {
  manualMode = (param.asInt() != 0);
  if (!manualMode) {
    // ensure auto takes over; turn off until motion
    setRelay(false);
  }
}
#endif

void setup() {
  Serial.begin(115200);
  delay(200);
  setupPins();
  ldrEma = analogRead(LDR_PIN);

  #if BLYNK_ENABLED
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting WiFi");
    while (WiFi.status() != WL_CONNECTED) { delay(300); Serial.print("."); }
    Serial.println();
    Blynk.begin(BLYNK_AUTH, WIFI_SSID, WIFI_PASS);
    Serial.print("IP: "); Serial.println(WiFi.localIP());
  #endif

  Serial.println(F("Smart Automatic Light - ESP8266"));
}

void loopAuto() {
  int ldr = analogRead(LDR_PIN);       // 0..1023
  ldrEma = LDR_SMOOTH_ALPHA * ldr + (1.0 - LDR_SMOOTH_ALPHA) * ldrEma;
  bool dark = (ldrEma > ldrThreshold); // Higher value means darker with this wiring

  int pir = digitalRead(PIR_PIN);      // 1 on motion
  if (pir == HIGH) {
    lastMotionMs = millis();
    #if BLYNK_ENABLED
      Blynk.virtualWrite(VPIN_MOTION, 1);
    #endif
  } else {
    #if BLYNK_ENABLED
      Blynk.virtualWrite(VPIN_MOTION, 0);
    #endif
  }

  bool shouldOn = dark && (millis() - lastMotionMs <= MOTION_HOLD_MS);
  if (shouldOn != relayOn) setRelay(shouldOn);

  #if BLYNK_ENABLED
    Blynk.virtualWrite(VPIN_LDR, (int)ldrEma);
  #endif
}

void loop() {
  #if BLYNK_ENABLED
    Blynk.run();
  #endif

  if (manualMode) {
    // Manual mode: Blynk switch controls relay; keep minimal processing
    delay(10);
    return;
  }

  loopAuto();
  delay(10);
}
