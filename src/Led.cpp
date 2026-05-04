#include <Arduino.h>
#include "State.hpp"

void setupLED(int PIN) {
  pinMode(PIN, OUTPUT); // Set pin as output
}

void toggleLED(int PIN) {
  digitalWrite(PIN, state.ledOn ? HIGH : LOW);
}