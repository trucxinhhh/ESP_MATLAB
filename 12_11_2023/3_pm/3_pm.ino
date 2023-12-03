#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "THUY NGA L3";
const char* password = "123456789";
const char* mqtt_server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

const char* mqttUserName_Control = "CiQhOig5PDAcFC0SEjU5LCs";
const char* mqttPass_Control = "5jUmQLZQxfY706dL1o1Q32ep";
const char* clientID_Control = "CiQhOig5PDAcFC0SEjU5LCs";
#define channelID 2332674
// #define fieldNumber 1

WiFiClient espClient;
PubSubClient client_Control(espClient);

void mqttSubscriptionCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqttSubscribe_Control() {
  Serial.println("have");
  String myTopic = "channels/" + String(channelID) + "/subscribe/fields/field" + String(1);
  client_Control.subscribe(myTopic.c_str());
}

void mqttConnect_Control() {
  while (!client_Control.connected()) {
    if (client_Control.connect(clientID_Control, mqttUserName_Control, mqttPass_Control)) {
      Serial.println("MQTT Connected");
      mqttSubscribe_Control();
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client_Control.state());
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
  client_Control.setServer(mqtt_server, mqttPort);
  client_Control.setCallback(mqttSubscriptionCallback);
}

void loop() {
  if (!client_Control.connected()) {
    mqttConnect_Control();
  }
  delay(10000);
  client_Control.loop();
}
