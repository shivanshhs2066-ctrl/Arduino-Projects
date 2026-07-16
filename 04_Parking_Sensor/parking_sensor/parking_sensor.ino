/*
  =====================================================================
  ARDUINO ULTRASONIC PARKING SENSOR
  =====================================================================
  Hardware:
    - HC-SR04 Ultrasonic Distance Sensor
    - 5641AS Common Cathode 4-Digit 7-Segment Display
    - 3 LEDs (Green / Yellow / Red)
    - Buzzer

  Behavior:
    - Measures distance in cm and shows it on the 4-digit display
    - > 50 cm   -> Green LED,  no buzzer
    - 20-50 cm  -> Yellow LED, slow beep
    - < 20 cm   -> Red LED,    fast beep

  The display is refreshed continuously using multiplexing (each digit
  is lit very briefly, one after another, fast enough that it looks
  like all 4 digits are on at once). No blocking delay() calls are
  used for the display or buzzer, so everything runs smoothly and
  the distance measurement doesn't cause flicker.

  NOTE ON PIN 1: Pin 1 on the Uno is also the hardware TX (Serial)
  pin. Using it for the Green LED means Serial.print() / the USB
  serial monitor may flicker the LED or misbehave while uploading
  or printing. This sketch does NOT use Serial for that reason.
  If you want serial debugging, move the Green LED to another pin.
  =====================================================================
*/

// ---------------------- HC-SR04 PINS --------------------------------
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

// ---------------------- LED PINS -------------------------------------
const int GREEN_LED  = 1;
const int YELLOW_LED = 2;
const int RED_LED    = 3;

// ---------------------- BUZZER PIN -----------------------------------
const int BUZZER_PIN = 8;

// ---------------------- 7-SEGMENT PINS --------------------------------
// Order: A, B, C, D, E, F, G
const int segPins[7] = {11, 13, A3, A4, A5, 12, A2};

// Digit (common cathode) control pins: Digit1(leftmost) .. Digit4(rightmost)
const int digitPins[4] = {5, 6, 7, 4};

// Segment patterns for digits 0-9.
// 1 = segment ON, 0 = segment OFF, in order A,B,C,D,E,F,G
const byte digitPatterns[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

// Holds the digit (0-9) currently shown on each of the 4 positions.
// Use -1 to mean "blank" (used to suppress leading zeros).
int displayDigits[4] = {-1, -1, -1, -1};

// ---------------------- TIMING VARIABLES ------------------------------
unsigned long lastMuxTime   = 0;   // last time we switched the active digit
const unsigned long muxInterval = 3; // ms between digit switches (fast = no flicker)
int currentDigit = 0;               // which of the 4 digits is currently lit

unsigned long lastDistanceTime = 0;
const unsigned long distanceInterval = 150; // ms between distance measurements

unsigned long lastBuzzerToggle = 0;
bool buzzerState = false;

long currentDistance = 0; // last measured distance in cm

// =====================================================================
void setup() {
  // Ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  // LED pins
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Segment pins
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
  }

  // Digit control pins
  // Common cathode: digit pin LOW = that digit's cathode is grounded = digit active
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // start with all digits OFF (HIGH = inactive)
  }
}

// =====================================================================
void loop() {
  unsigned long now = millis();

  // ---- 1. Measure distance periodically (non-blocking schedule) ----
  if (now - lastDistanceTime >= distanceInterval) {
    lastDistanceTime = now;
    currentDistance = measureDistanceCM();
    updateDisplayDigits(currentDistance);
    updateLEDsAndBuzzerMode(currentDistance);
  }

  // ---- 2. Handle buzzer beeping pattern (non-blocking) ----
  handleBuzzer(now);

  // ---- 3. Refresh the 7-segment display (multiplexing, non-blocking) ----
  refreshDisplay(now);
}

// =====================================================================
// Measures distance using the HC-SR04 and returns the value in cm.
// Uses pulseIn() with a timeout so it can't hang forever.
// =====================================================================
long measureDistanceCM() {
  // Send a 10us trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo pulse duration (timeout after ~25ms ~= 430cm max range)
  long duration = pulseIn(ECHO_PIN, HIGH, 25000);

  if (duration == 0) {
    // No echo received (out of range) -> treat as "far away"
    return 999;
  }

  // Speed of sound ~343 m/s -> distance(cm) = duration(us) / 58
  long distance = duration / 58;
  return distance;
}

// =====================================================================
// Breaks the distance value into individual digits for the display,
// suppressing leading zeros (but always showing at least the units digit).
// Example: 25 -> shows "  25" (blank, blank, 2, 5)
// =====================================================================
void updateDisplayDigits(long distance) {
  // Clamp to a displayable range (0-9999)
  if (distance > 9999) distance = 9999;
  if (distance < 0) distance = 0;

  int d[4];
  d[0] = (distance / 1000) % 10; // thousands
  d[1] = (distance / 100) % 10;  // hundreds
  d[2] = (distance / 10) % 10;   // tens
  d[3] = distance % 10;          // units

  // Suppress leading zeros (leftmost digits), keep at least the units digit
  bool leadingZero = true;
  for (int i = 0; i < 3; i++) { // check positions 0,1,2 (not the last/units digit)
    if (leadingZero && d[i] == 0) {
      displayDigits[i] = -1; // blank
    } else {
      leadingZero = false;
      displayDigits[i] = d[i];
    }
  }
  displayDigits[3] = d[3]; // units digit always shown
}

// =====================================================================
// Sets LED states and decides which buzzer "mode" should be active
// based on the measured distance.
// =====================================================================
enum BuzzerMode { BUZZ_OFF, BUZZ_SLOW, BUZZ_FAST };
BuzzerMode currentBuzzerMode = BUZZ_OFF;

void updateLEDsAndBuzzerMode(long distance) {
  if (distance > 50) {
    // Far away
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    currentBuzzerMode = BUZZ_OFF;
  } else if (distance >= 20) {
    // Medium distance (20-50 cm)
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    currentBuzzerMode = BUZZ_SLOW;
  } else {
    // Close (< 20 cm)
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    currentBuzzerMode = BUZZ_FAST;
  }
}

// =====================================================================
// Handles buzzer beeping without using delay(), based on currentBuzzerMode.
// =====================================================================
void handleBuzzer(unsigned long now) {
  unsigned long interval;

  switch (currentBuzzerMode) {
    case BUZZ_OFF:
      digitalWrite(BUZZER_PIN, LOW);
      return;
    case BUZZ_SLOW:
      interval = 500; // slow beep: toggle every 500ms
      break;
    case BUZZ_FAST:
      interval = 100; // fast beep: toggle every 100ms
      break;
  }

  if (now - lastBuzzerToggle >= interval) {
    lastBuzzerToggle = now;
    buzzerState = !buzzerState;
    digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
  }
}

// =====================================================================
// Refreshes the 7-segment display using multiplexing.
// Each call (roughly every muxInterval ms) turns off the previous digit,
// sets the segments for the next digit, and turns that digit on.
// Cycling through all 4 digits fast enough makes them appear lit
// simultaneously to the human eye (no visible flicker).
// =====================================================================
void refreshDisplay(unsigned long now) {
  if (now - lastMuxTime < muxInterval) {
    return; // not time to switch digits yet
  }
  lastMuxTime = now;

  // Turn off all digits first (avoid ghosting between digits)
  for (int i = 0; i < 4; i++) {
    digitalWrite(digitPins[i], HIGH); // HIGH = inactive for common cathode w/ transistor-less direct drive
  }

  int value = displayDigits[currentDigit];

  if (value >= 0 && value <= 9) {
    // Set segment pins according to the digit pattern
    for (int s = 0; s < 7; s++) {
      digitalWrite(segPins[s], digitPatterns[value][s] ? HIGH : LOW);
    }
    // Activate this digit (LOW = active, common cathode)
    digitalWrite(digitPins[currentDigit], LOW);
  } else {
    // Blank digit: turn all segments off
    for (int s = 0; s < 7; s++) {
      digitalWrite(segPins[s], LOW);
    }
    // Leave digit pin HIGH (inactive) so nothing shows
  }

  // Move to the next digit for the next refresh cycle
  currentDigit++;
  if (currentDigit > 3) currentDigit = 0;
}
