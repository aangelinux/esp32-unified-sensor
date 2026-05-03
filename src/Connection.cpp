#include <WiFi.h>
#include <PubSubClient.h>

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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    if (client.connect("ESP32Client")) {
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

  // Create string payload with data here

  client.loop();
  client.publish("lnu/iot/al227bn/sensor", "");
  delay(5000);
}

bool subscribe() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  bool state = client.subscribe("lnu/iot/al227bn/command/led");
  delay(5000);
  
  return state;
}