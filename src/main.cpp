#include <Arduino.h>
#include "Led.hpp"
#include "Sensor.hpp"
#include "Connection.hpp"
#include "State.hpp"

unsigned long timeSincePublish;

void setup() {
  setupLED(2);
  setupSensor();
  setupConnection();
}

void loop() {
  if (state.ledUpdated) {
    toggleLED(2);
  }

  if ((millis() - timeSincePublish) > 5000) {
    timeSincePublish = millis();

    float temperature = getTemperature();
    float humidity = getHumidity();

    publish(temperature, humidity);
  }
}