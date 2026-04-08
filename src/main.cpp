// GV2 Detection LED Controller
// Parses JSON detection data from Grove Vision AI V2 and controls LEDs based on detected classes

#include <Arduino.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>

// UART for Grove Vision AI V2
// Use a dedicated UART peripheral for GV2 (pins are set in begin()).
HardwareSerial gv2Serial(1);

// LED pins
const int ledPinGreen = 2;   // GPIO 2 (D1) - Apis mellifera (class 0)
const int ledPinYellow = 3; // GPIO 3 (D2) - (optional / unused in fast mode)
const int ledPinRed = 4;     // GPIO 4 (D3) - Vespa velutina (class 3)

// Fast-boot tuning
static const uint32_t NO_DATA_TIMEOUT_MS = 1500;

// Lightweight parser state for extracting class IDs from `"boxes": [[...]]`
static bool s_invokeSeen = false;
static bool s_inBoxes = false;
static uint8_t s_boxesBracketDepth = 0; // depth within boxes payload starting at first '[' after "boxes":
static bool s_inBox = false;
static int s_boxValueIndex = -1; // counts numeric values within a single box array; class is index 5
static int s_currentNumber = 0;
static bool s_inNumber = false;

static bool s_hasOther = false; // classes 0/1/2 -> green
static bool s_hasHornet = false; // class 3 -> red

static inline void resetParseState() {
  s_invokeSeen = false;
  s_inBoxes = false;
  s_boxesBracketDepth = 0;
  s_inBox = false;
  s_boxValueIndex = -1;
  s_currentNumber = 0;
  s_inNumber = false;
}

static inline void resetDetections() {
  s_hasOther = false;
  s_hasHornet = false;
}

static inline bool charMatchesAt(const char *pattern, size_t &idx, char c) {
  if (c == pattern[idx]) {
    idx++;
    if (pattern[idx] == '\0') {
      idx = 0;
      return true;
    }
  } else {
    idx = (c == pattern[0]) ? 1 : 0;
  }
  return false;
}

static inline void feedCharToFastParser(char c) {
  // Match minimal markers. We only care about INVOKE and boxes.
  static size_t invokeIdx = 0;
  static size_t boxesIdx = 0;
  static size_t typeIdx = 0;

  // New message heuristic: reset detections at the start of each INVOKE JSON blob.
  // This avoids "sticky" LEDs across messages.
  if (charMatchesAt("{\"type\": 1", typeIdx, c)) {
    resetDetections();
    resetParseState();
    // We just matched a prefix; continue parsing from here.
    s_invokeSeen = true;
  }

  if (!s_invokeSeen) {
    // `"name": "INVOKE"` appears early; once seen, we start looking for boxes and numbers.
    if (charMatchesAt("\"name\": \"INVOKE\"", invokeIdx, c)) {
      s_invokeSeen = true;
    }
    return;
  }

  if (!s_inBoxes) {
    if (charMatchesAt("\"boxes\":", boxesIdx, c)) {
      s_inBoxes = true;
      s_boxesBracketDepth = 0;
      s_inBox = false;
      s_boxValueIndex = -1;
      s_inNumber = false;
      s_currentNumber = 0;
    }
    return;
  }

  // We are inside boxes: parse bracket structure and integers.
  if (c == '[') {
    if (s_boxesBracketDepth < 255) s_boxesBracketDepth++;
    // boxes looks like: [[box],[box],...]
    // Depth 1: first '[' after "boxes":
    // Depth 2: second '[' (start of first box)
    // Depth 3+: nested arrays (not expected)
    if (s_boxesBracketDepth == 2) {
      s_inBox = true;
      s_boxValueIndex = -1;
      s_inNumber = false;
      s_currentNumber = 0;
    }
    return;
  }

  if (c == ']') {
    if (s_inNumber) {
      // finalize number before closing bracket
      s_boxValueIndex++;
      if (s_inBox && s_boxValueIndex == 5) {
        if (s_currentNumber == 3) s_hasHornet = true;
        else s_hasOther = true;
      }
      s_inNumber = false;
      s_currentNumber = 0;
    }

    if (s_boxesBracketDepth > 0) s_boxesBracketDepth--;
    if (s_boxesBracketDepth < 2) {
      s_inBox = false;
      s_boxValueIndex = -1;
    }
    return;
  }

  // Parse integers only while inside a box.
  if (!s_inBox) return;

  if (c >= '0' && c <= '9') {
    s_currentNumber = s_inNumber ? (s_currentNumber * 10 + (c - '0')) : (c - '0');
    s_inNumber = true;
    return;
  }

  if (s_inNumber) {
    // Number ended; advance value index.
    s_boxValueIndex++;
    if (s_boxValueIndex == 5) {
      // class id
      if (s_currentNumber == 3) s_hasHornet = true;
      else s_hasOther = true;
    }
    s_inNumber = false;
    s_currentNumber = 0;
  }
}

void setup() {
  // Optional USB serial (do not wait; fastest boot)
  Serial.begin(115200);

  // Configure UART for GV2 communication
  // RX: GPIO44 (D7), TX: GPIO43 (D6), Baud: 115200
  gv2Serial.begin(115200, SERIAL_8N1, 44, 43);
  
  // Set larger buffer for UART reception
  gv2Serial.setRxBufferSize(2048);
  
  // Initialize LED pins
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinRed, OUTPUT);

  digitalWrite(ledPinGreen, LOW);
  digitalWrite(ledPinYellow, LOW);
  digitalWrite(ledPinRed, LOW);

  resetParseState();
  resetDetections();
}

void loop() {
  static unsigned long lastDataTime = 0;

  // Read incoming stream from GV2 and update detection flags asap.
  while (gv2Serial.available() > 0) {
    char c = gv2Serial.read();
    lastDataTime = millis();
    feedCharToFastParser(c);

    // Apply LEDs immediately based on latest seen detections.
    digitalWrite(ledPinGreen, s_hasOther ? HIGH : LOW);
    digitalWrite(ledPinRed, s_hasHornet ? HIGH : LOW);
  }

  // Timeout: if stream stops, clear LEDs and reset parser/detections.
  if (lastDataTime != 0 && (millis() - lastDataTime > NO_DATA_TIMEOUT_MS)) {
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinRed, LOW);
    resetParseState();
    resetDetections();
    lastDataTime = 0;
  }
  // No delay: maximize responsiveness.
}

