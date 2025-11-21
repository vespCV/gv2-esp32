#include <Arduino.h>

const int ledPinGreen = 2; // GPIO 2 (D1)
const int ledPinYellow = 3; // GPIO 3 (D2)
const int ledPinRed = 4; // GPIO 4 (D3)

void setup() {
  // 1. Initialiseer de pin als een digitale uitgang.
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinRed, OUTPUT);
  
  // 2. FORCEER de pin ONMIDDELLIJK naar LOW (UIT)
  digitalWrite(ledPinGreen, LOW);
  digitalWrite(ledPinYellow, LOW);
  digitalWrite(ledPinRed, LOW);
}

void loop() {
  // LED KNIPPENEN
  digitalWrite(ledPinGreen, HIGH);
  delay(500);
  digitalWrite(ledPinGreen, LOW);
  digitalWrite(ledPinYellow, HIGH);
  delay(500);
  digitalWrite(ledPinYellow, LOW);
  digitalWrite(ledPinRed, HIGH);
  delay(500);
  digitalWrite(ledPinRed, LOW);
}