#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "State.hpp"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.emqx.io";
int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, message);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  const char* command = doc["msg"];
  if (strcmp(topic, "lnu/iot/al227bn/command/led") == 0) {
    if (strcmp(command, "ON") == 0) {
      state.ledOn = true;
      state.ledUpdated = true;
    } else if (strcmp(command, "OFF") == 0) {
      state.ledOn = false;
      state.ledUpdated = true;
    } else {
      Serial.print("Invalid command: "); Serial.println(command);
    }
  }
}


void setupConnection() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");
  client.setServer(mqttServer, mqttPort);
}

void connect() {
  if (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    if (client.connect("ESP32UnifiedSensor")) {
      client.subscribe("lnu/iot/al227bn/command/led");
      client.setCallback(callback);
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc = ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void loopConnection() {
  if (!client.connected()) {
    connect();
  }
  
  client.loop();
}

void publish(float temperature, float humidity) {
  JsonDocument data;
  data["temperature"] = temperature;
  data["humidity"] = humidity;

  char output[256];
  serializeJson(data, output);

  client.publish("lnu/iot/al227bn/sensor", output);
}