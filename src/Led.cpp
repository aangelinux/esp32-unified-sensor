#include <Arduino.h>

void setupLED(int PIN) {
  pinMode(PIN, OUTPUT); // Set pin as output
}

void toggleLED(int PIN, bool state) {
  digitalWrite(PIN, state ? HIGH : LOW);
}