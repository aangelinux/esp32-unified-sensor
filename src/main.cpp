#include <Arduino.h>
#include "Led.hpp"
#include "Sensor.hpp"
#include "Connection.hpp"

void setup() {
  setupLED(2);
  setupSensor();
  setupConnection();
}

void loop() {
  bool state = subscribe();

  toggleLED(2, state);

  float temperature = getTemperature();
  float humidity = getHumidity();

  publish(temperature, humidity);
}