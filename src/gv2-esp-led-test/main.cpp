#include <Seeed_Arduino_SSCMA.h>

#ifdef ESP32
#include <HardwareSerial.h>

// Define two Serial devices mapped to the two internal UARTs
HardwareSerial atSerial(0);

#else
#define atSerial Serial1
#endif

// LED pin definitions for XIAO ESP32-S3
const int ledPinGreen = 2;   // GPIO 2 (D1) - Apis mellifera
const int ledPinYellow = 3;  // GPIO 3 (D2) - Vespa crabro, Vespula sp
const int ledPinRed = 4;     // GPIO 4 (D3) - Vespa velutina

SSCMA AI;

void setup() {
  AI.begin( & atSerial);
  Serial.begin(9600);
  
  // Initialize LED pins as outputs and turn them off
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  digitalWrite(ledPinGreen, LOW);
  digitalWrite(ledPinYellow, LOW);
  digitalWrite(ledPinRed, LOW);
}

void loop() {
  if (!AI.invoke(1, false, false)) { // invoke once, no filter, not contain image
    Serial.println("invoke success");
    Serial.print("perf: prepocess=");
    Serial.print(AI.perf().prepocess);
    Serial.print(", inference=");
    Serial.print(AI.perf().inference);
    Serial.print(", postpocess=");
    Serial.println(AI.perf().postprocess);

    // Turn off all LEDs first
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinYellow, LOW);
    digitalWrite(ledPinRed, LOW);
    
    // Find the class with the highest score from boxes (object detection)
    int detectedClass = -1;
    float maxScore = 0.0;
    
    for (int i = 0; i < AI.boxes().size(); i++) {
      Serial.print("Box[");
      Serial.print(i);
      Serial.print("] target=");
      Serial.print(AI.boxes()[i].target);
      Serial.print(", score=");
      Serial.print(AI.boxes()[i].score);
      Serial.print(", x=");
      Serial.print(AI.boxes()[i].x);
      Serial.print(", y=");
      Serial.print(AI.boxes()[i].y);
      Serial.print(", w=");
      Serial.print(AI.boxes()[i].w);
      Serial.print(", h=");
      Serial.println(AI.boxes()[i].h);
      
      // Track the box with highest confidence score
      if (AI.boxes()[i].score > maxScore) {
        maxScore = AI.boxes()[i].score;
        detectedClass = AI.boxes()[i].target;
      }
    }
    
    // If no boxes found, check classes (classification)
    if (detectedClass == -1) {
      for (int i = 0; i < AI.classes().size(); i++) {
        Serial.print("Class[");
        Serial.print(i);
        Serial.print("] target=");
        Serial.print(AI.classes()[i].target);
        Serial.print(", score=");
        Serial.println(AI.classes()[i].score);
        
        // Track the class with highest confidence score
        if (AI.classes()[i].score > maxScore) {
          maxScore = AI.classes()[i].score;
          detectedClass = AI.classes()[i].target;
        }
      }
    }
    
    // Control LEDs based on detected class
    // Class 0: Apis mellifera -> Green LED
    // Class 1: Vespa crabro -> Yellow LED
    // Class 2: Vespula sp -> Yellow LED
    // Class 3: Vespa velutina -> Red LED
    if (detectedClass == 0) {
      digitalWrite(ledPinGreen, HIGH);
      Serial.println("LED: Green (Apis mellifera detected)");
      delay(3000); // Keep LED on for 3 seconds
      digitalWrite(ledPinGreen, LOW);
    } else if (detectedClass == 1 || detectedClass == 2) {
      digitalWrite(ledPinYellow, HIGH);
      Serial.print("LED: Yellow (Class ");
      Serial.print(detectedClass);
      Serial.println(" detected)");
      delay(3000); // Keep LED on for 3 seconds
      digitalWrite(ledPinYellow, LOW);
    } else if (detectedClass == 3) {
      digitalWrite(ledPinRed, HIGH);
      Serial.println("LED: Red (Vespa velutina detected)");
      delay(3000); // Keep LED on for 3 seconds
      digitalWrite(ledPinRed, LOW);
    }
    for (int i = 0; i < AI.points().size(); i++) {
      Serial.print("Point[");
      Serial.print(i);
      Serial.print("]: target=");
      Serial.print(AI.points()[i].target);
      Serial.print(", score=");
      Serial.print(AI.points()[i].score);
      Serial.print(", x=");
      Serial.print(AI.points()[i].x);
      Serial.print(", y=");
      Serial.println(AI.points()[i].y);
    }
    for (int i = 0; i < AI.keypoints().size(); i++) {
      Serial.print("keypoint[");
      Serial.print(i);
      Serial.print("] target=");
      Serial.print(AI.keypoints()[i].box.target);
      Serial.print(", score=");
      Serial.print(AI.keypoints()[i].box.score);
      Serial.print(", box:[x=");
      Serial.print(AI.keypoints()[i].box.x);
      Serial.print(", y=");
      Serial.print(AI.keypoints()[i].box.y);
      Serial.print(", w=");
      Serial.print(AI.keypoints()[i].box.w);
      Serial.print(", h=");
      Serial.print(AI.keypoints()[i].box.h);
      Serial.print("], points:[");
      for (int j = 0; j < AI.keypoints()[i].points.size(); j++) {
        Serial.print("[");
        Serial.print(AI.keypoints()[i].points[j].x);
        Serial.print(",");
        Serial.print(AI.keypoints()[i].points[j].y);
        Serial.print("],");
      }
      Serial.println("]");
    }
  }
}