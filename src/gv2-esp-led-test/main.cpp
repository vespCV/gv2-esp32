// GV2 Detection LED Controller
// Parses JSON detection data from Grove Vision AI V2 and controls LEDs based on detected classes

#include <Arduino.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>

// UART for Grove Vision AI V2
HardwareSerial gv2Serial(0);

// LED pins
const int ledPinGreen = 2;   // GPIO 2 (D1) - Apis mellifera (class 0)
const int ledPinYellow = 3; // GPIO 3 (D2) - Vespa crabro (class 1) and Vespula sp (class 2)
const int ledPinRed = 4;     // GPIO 4 (D3) - Vespa velutina (class 3)

void setup() {
  // USB Serial for debugging
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    ; // Wait for serial port to connect, max 3 seconds
  }
  
  Serial.println("\n\n=== GV2 UART1 Debug Test ===");
  Serial.println("Initializing...");
  
  // Configure UART for GV2 communication
  // RX: GPIO44 (D7), TX: GPIO43 (D6), Baud: 115200
  gv2Serial.begin(115200, SERIAL_8N1, 44, 43);
  
  // Set larger buffer for UART reception
  gv2Serial.setRxBufferSize(2048);
  
  Serial.println("UART1 configured:");
  Serial.println("  RX: GPIO44 (D7)");
  Serial.println("  TX: GPIO43 (D6)");
  Serial.println("  Baud: 115200");
  Serial.println("  RX Buffer: 2048 bytes");
  Serial.println("\nWaiting for data from GV2...\n");
  
  // Test: Try to read any initial data that might be in buffer
  delay(100);
  if (gv2Serial.available() > 0) {
    Serial.println(">>> Found data in buffer immediately after init!");
  }
  
  // Initialize LED pins
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  
  // Blink all LEDs to show system is running
  digitalWrite(ledPinGreen, HIGH);
  digitalWrite(ledPinYellow, HIGH);
  digitalWrite(ledPinRed, HIGH);
  delay(500);
  digitalWrite(ledPinGreen, LOW);
  digitalWrite(ledPinYellow, LOW);
  digitalWrite(ledPinRed, LOW);
  
  Serial.println("=== Ready to receive ===\n");
  Serial.println("LED Mapping:");
  Serial.println("  Green (GPIO2/D1)  -> Apis mellifera (class 0)");
  Serial.println("  Yellow (GPIO3/D2)  -> Vespa crabro (class 1) or Vespula sp (class 2)");
  Serial.println("  Red (GPIO4/D3)     -> Vespa velutina (class 3)");
  Serial.println();
}

// Forward declaration
void processDetectionJSON(String jsonString);

void loop() {
  static String jsonBuffer = "";
  static unsigned long lastDataTime = 0;
  static unsigned long lastStatsTime = 0;
  
  // Read incoming data from GV2
  while (gv2Serial.available() > 0) {
    char c = gv2Serial.read();
    
    // Accumulate characters until we get a complete JSON line (ends with \n)
    if (c == '\n' || c == '\r') {
      if (jsonBuffer.length() > 0) {
        // Try to parse JSON
        processDetectionJSON(jsonBuffer);
        jsonBuffer = "";
      }
    } else {
      jsonBuffer += c;
      // Prevent buffer overflow
      if (jsonBuffer.length() > 2048) {
        jsonBuffer = "";
      }
    }
    
    lastDataTime = millis();
  }
  
  // Turn off LEDs if no data received for 2 seconds (timeout)
  if (millis() - lastDataTime > 2000) {
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinYellow, LOW);
    digitalWrite(ledPinRed, LOW);
  }
  
  // Print statistics every 5 seconds
  if (millis() - lastStatsTime > 5000) {
    lastStatsTime = millis();
    if (lastDataTime == 0 || (millis() - lastDataTime > 5000)) {
      Serial.println(">>> Waiting for detection data from GV2...");
    } else {
      Serial.println(">>> System active - receiving detections");
    }
  }
  
  delay(10);
}

void processDetectionJSON(String jsonString) {
  // Filter: Only process INVOKE messages (type 1)
  if (!jsonString.startsWith("{\"type\": 1")) {
    return;
  }
  
  // Parse JSON
  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, jsonString);
  
  if (error) {
    Serial.printf(">>> JSON parse error: %s\n", error.c_str());
    return;
  }
  
  // Check if this is an INVOKE message
  if (doc["type"] != 1 || doc["name"] != "INVOKE") {
    return;
  }
  
  // Get the data object
  JsonObject data = doc["data"];
  if (data.isNull()) {
    return;
  }
  
  // Get count and boxes array
  int count = data["count"] | 0;
  JsonArray boxes = data["boxes"];
  
  // Track which classes are detected
  bool hasClass0 = false; // Apis mellifera (bees) -> Green
  bool hasClass1 = false; // Vespa crabro -> Yellow
  bool hasClass2 = false; // Vespula sp -> Yellow
  bool hasClass3 = false; // Vespa velutina -> Red
  
  // Process each detection box
  // Format: [x, y, w, h, score, target]
  // target is the class ID (index 5 in the array)
  for (size_t i = 0; i < boxes.size(); i++) {
    JsonArray box = boxes[i];
    if (box.size() >= 6) {
      int target = box[5]; // Class ID is at index 5
      
      switch (target) {
        case 0:
          hasClass0 = true;
          break;
        case 1:
          hasClass1 = true;
          break;
        case 2:
          hasClass2 = true;
          break;
        case 3:
          hasClass3 = true;
          break;
      }
    }
  }
  
  // Control LEDs based on detected classes
  digitalWrite(ledPinGreen, hasClass0 ? HIGH : LOW);
  digitalWrite(ledPinYellow, (hasClass1 || hasClass2) ? HIGH : LOW);
  digitalWrite(ledPinRed, hasClass3 ? HIGH : LOW);
  
  // Debug output
  if (count > 0) {
    Serial.printf(">>> Detection: count=%d, Class0=%s, Class1=%s, Class2=%s, Class3=%s\n",
                  count,
                  hasClass0 ? "YES" : "NO",
                  hasClass1 ? "YES" : "NO",
                  hasClass2 ? "YES" : "NO",
                  hasClass3 ? "YES" : "NO");
  }
}

