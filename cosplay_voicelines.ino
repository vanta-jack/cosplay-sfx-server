#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Ash realme C21Y";         // Your Wi-Fi SSID
const char* password = "12345888"; // Your Wi-Fi password

// Replace with your laptop's current IP address
const char* serverBaseUrl = "http://10.86.204.245:5000/play?track=";

const int button1Pin = 4; // Button 1 on GPIO 4 (Multi-action: Tracks 1, 2, 3)
const int button2Pin = 5; // Button 2 on GPIO 5 (Ultimate: Track 4 + Charging LEDs)

const int CHARGE_LEDS[4] = {25, 26, 27, 14}; // 4-LED charging sequence pins
const int SCOPE_LED = 32; // Scope light on GPIO 32 (single LED for now, 3W LED + MOSFET later)

// Timing & State variables for Button 1 (Multi-click & Long press)
unsigned long pressStartTime1 = 0;
unsigned long lastReleaseTime1 = 0;
int clickCount1 = 0;
bool isHolding1 = false;
bool buttonWasPressed1 = false;

const unsigned long longPressTime = 2000;    // 2 seconds for long press
const unsigned long doubleClickWindow = 350; // Window to register second click

// Timing & State variables for Button 2 (Ultimate / Charging & Shooting Sequence)
bool isFiring = false;
unsigned long sequenceStartTime = 0;

const unsigned long AUDIO_DURATION_MS = 8000; // Total audio length 

// 1. Initial Charging Phase Timing
const unsigned long CHARGE_START_DELAY_MS = 2400; // Wait before initial charge starts
const unsigned long CHARGE_TOTAL_MS = 2400;       // Time to step through initial 4 LEDs

// 2. Rapid Shot & Final Flash Phase Timing
const unsigned long SHOT_FLASH_DURATION = 300;    // Fast sweep duration per flash (0.3 seconds)

// Exact timestamps for the 4 shots + final post-shot flash
const unsigned long SHOT_TIMES[5] = { 4200, 5000, 5800, 6600, 7400 };

void setup() {
  Serial.begin(115200);

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    pinMode(CHARGE_LEDS[i], OUTPUT);
    digitalWrite(CHARGE_LEDS[i], LOW);
  }

  pinMode(SCOPE_LED, OUTPUT);
  digitalWrite(SCOPE_LED, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected! Dual-button mode ready.");
}

void loop() {
  unsigned long currentMillis = millis();

  // ==========================================
  // BUTTON 1 LOGIC (GPIO 4: Tracks 1, 2, and 3)
  // ==========================================
  bool button1State = (digitalRead(button1Pin) == LOW);

  if (button1State && !buttonWasPressed1) {
    buttonWasPressed1 = true;
    pressStartTime1 = currentMillis;
    isHolding1 = false;
  }

  if (button1State && buttonWasPressed1) {
    if (!isHolding1 && (currentMillis - pressStartTime1 >= longPressTime)) {
      isHolding1 = true;
      Serial.println("Button 1: Long press detected! Playing Track 3...");
      sendHttpRequest("3");
    }
  }

  if (!button1State && buttonWasPressed1) {
    buttonWasPressed1 = false;

    if (!isHolding1) {
      unsigned long pressDuration = currentMillis - pressStartTime1;

      if (pressDuration > 30) {
        if (clickCount1 == 1 && (currentMillis - lastReleaseTime1 <= doubleClickWindow)) {
          clickCount1 = 0;
          Serial.println("Button 1: Double click detected! Playing Track 2...");
          sendHttpRequest("2");
        } else {
          clickCount1 = 1;
          lastReleaseTime1 = currentMillis;
        }
      }
    }
  }

  if (clickCount1 == 1 && (currentMillis - lastReleaseTime1 > doubleClickWindow)) {
    clickCount1 = 0;
    Serial.println("Button 1: Single click detected! Playing Track 1...");
    sendHttpRequest("1");
  }

  // ==========================================================
  // BUTTON 2 LOGIC (GPIO 5: Ultimate Track 4 + Animations)
  // ==========================================================
  bool button2State = (digitalRead(button2Pin) == LOW);

  if (!isFiring && button2State) {
    delay(30); // Debounce
    if (digitalRead(button2Pin) == LOW) {
      isFiring = true;
      sequenceStartTime = currentMillis;
      digitalWrite(SCOPE_LED, HIGH); // Scope light ON from the button press
      Serial.println("Button 2: Ultimate triggered! Alternate voice line + lesley_ult + LEDs...");
      sendHttpRequest("4"); // Request Track 4 (Python handles alternation)
    }
  }

  // Run the charging and shooting LED sequence when Button 2 is active
  if (isFiring) {
    unsigned long elapsed = currentMillis - sequenceStartTime;
    bool ledsShouldBeOn = false;

    // 1. Initial Charging Sequence Phase
    if (elapsed >= CHARGE_START_DELAY_MS && elapsed < (CHARGE_START_DELAY_MS + CHARGE_TOTAL_MS)) {
      unsigned long chargeElapsed = elapsed - CHARGE_START_DELAY_MS;
      unsigned long stepInterval = CHARGE_TOTAL_MS / 4;

      for (int i = 0; i < 4; i++) {
        if (chargeElapsed >= (stepInterval * i)) {
          digitalWrite(CHARGE_LEDS[i], HIGH);
          ledsShouldBeOn = true;
        }
      }
    }

    // 2. Rapid Shot Flashes & Final Flash Phase (Loops through all 5 timestamps)
    for (int shot = 0; shot < 5; shot++) {
      unsigned long shotStart = SHOT_TIMES[shot];
      unsigned long shotEnd = shotStart + SHOT_FLASH_DURATION;

      if (elapsed >= shotStart && elapsed < shotEnd) {
        unsigned long shotElapsed = elapsed - shotStart;
        unsigned long fastStepInterval = SHOT_FLASH_DURATION / 4;

        for (int i = 0; i < 4; i++) {
          if (shotElapsed >= (fastStepInterval * i)) {
            digitalWrite(CHARGE_LEDS[i], HIGH);
            ledsShouldBeOn = true;
          }
        }
      }
    }

    // If none of the active lighting windows are met, ensure all LEDs stay turned OFF
    if (!ledsShouldBeOn) {
      for (int i = 0; i < 4; i++) {
        digitalWrite(CHARGE_LEDS[i], LOW);
      }
    }

    // Safety fallback: End sequence when audio duration finishes
    if (elapsed >= AUDIO_DURATION_MS) {
      for (int i = 0; i < 4; i++) {
        digitalWrite(CHARGE_LEDS[i], LOW);
      }
      digitalWrite(SCOPE_LED, LOW); // Scope light OFF when the sequence ends
      isFiring = false;
    }
  }
}

void sendHttpRequest(String trackNum) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String fullUrl = String(serverBaseUrl) + trackNum;

    http.begin(fullUrl.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
