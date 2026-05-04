#include <WiFi.h>
#include <PubSubClient.h>
#include "State.hpp"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.emqx.io";
int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

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

void callback(char* topic, byte* payload, unsigned int length) {
  String message;

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (topic == "lnu/iot/al227bn/command/led") {
    state.ledUpdated = true;
    
    if (message == "ON") {
      state.ledOn = true;
    } else {
      state.ledOn = false;
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    if (client.connect("ESP32Client")) {
      client.subscribe("lnu/iot/al227bn/command/led");
      client.setCallback(callback);
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void publish(float temperature, float humidity) {
  if (!client.connected()) {
    reconnect();
  }

  String data = 
    "Temperature: " + String(temperature) + 
    "\nHumidity: " + String(humidity);

  client.loop();
  client.publish("lnu/iot/al227bn/sensor", data.c_str());
}