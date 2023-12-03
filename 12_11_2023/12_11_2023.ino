#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "THUY NGA L3";
const char* password = "123456789";
const char* mqtt_server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

const char* mqttUserName_Garden = "DDsvAh8eGgg8NhogHRsnIhs";
const char* mqttPass_Garden = "pdyApvLd+VsGFBwjbDIDZlXB";
const char* clientID_Garden = "DDsvAh8eGgg8NhogHRsnIhs";

#define channelID_Garden 2327722



WiFiClient espClient;
PubSubClient client_Garden(espClient);



void mqttPublish_Garden(long pubChannelID, String message) {
  String topicString = "channels/" + String(pubChannelID) + "/publish";
  client_Garden.publish(topicString.c_str(), message.c_str());
}

void mqttConnect_Garden() {
  while (!client_Garden.connected()) {
    if (client_Garden.connect(clientID_Garden, mqttUserName_Garden, mqttPass_Garden)) {
      Serial.println("MQTT Garden Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client_Garden.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void connectWiFi() {
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  client_Garden.setServer(mqtt_server, mqttPort);
}

void loop() {
  if (!client_Garden.connected()) {
    mqttConnect_Garden();
  }
  String message = "field1=" + String(10) + '&' +"field2=" + String(20);
  mqttPublish_Garden(channelID_Garden, message);
  Serial.println("pppp");
  delay(10000);

}
