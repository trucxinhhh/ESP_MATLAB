#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <DHT.h>

float err_measure = 2;    // Độ lệch khi đo
float err_estimate = 2;   // Độ lệch ước lượng
float q = 0.001;          // Độ lớn của nhiễu
float last_estimate = 100;  //ước lượng lúc cuối

const char* ssid = "THUY NGA L3";
const char* password = "123456789";
const char* mqtt_server = "mqtt3.thingspeak.com";
const int mqttPort = 1883;

const char* mqttUserName_Control = "CiQhOig5PDAcFC0SEjU5LCs";
const char* mqttPass_Control = "5jUmQLZQxfY706dL1o1Q32ep";
const char* clientID_Control = "CiQhOig5PDAcFC0SEjU5LCs";
#define channelID_Control 2332674
#define api_key_write "NXGPB1CDKCPB9H9U"
WiFiClient espClient;
HTTPClient http;

PubSubClient client_Control(espClient);

const int soilPin = A0;
#define relay1 5   
DHT dht(4, DHT11);   // Kết nối cảm biến DHT11 vào chân 4

float KalmanFilter(float mea) {
  float kalman_gain = err_estimate / (err_estimate + err_measure);                                   // Hệ số Kalman = Độ lệch khi đo / (Độ lệch ước lượng + Độ lệch khi đo)
  float current_estimate = last_estimate + kalman_gain * (mea - last_estimate);                      // Ước lượng hiện tại = ước lượng lúc cuối + hệ số Kalman * (Giá trị đo được - ước lượng lúc cuối)
  err_estimate = (1.0f - kalman_gain) * err_estimate + fabsf(last_estimate - current_estimate) * q;  // Độ lệch ước lượng = (1 - hệ số Kalman) * Độ lệch ước lượng + |ước lượng lúc cuối - ước lượng hiện tại| * Độ lớn của nhiễu
  last_estimate = current_estimate;                                                                  // Uớc lượng lúc cuối = Ước lượng hiện tại

  return current_estimate;
}

void mqttSubscriptionCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    // Serial.print((char)payload[i]);
    char control1 = (char)payload[i];
    Serial.print(control1);
    int control = control1-'0';
    if (control == 1){
      digitalWrite(relay1,HIGH);
    }
    else {
      digitalWrite(relay1, LOW);
    }
  }
  Serial.println();
}

void mqttSubscribe_Control() {
  Serial.println("have");
  String myTopic = "channels/" + String(channelID_Control) + "/subscribe/fields/field" + String(1);
  client_Control.subscribe(myTopic.c_str());
}

void mqttConnect_Control() {
  while (!client_Control.connected()) {
    if (client_Control.connect(clientID_Control, mqttUserName_Control, mqttPass_Control)) {
      Serial.println("MQTT Control Connected");
      mqttSubscribe_Control();
    } else {
      Serial.print("Failed Control, rc=");
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
  pinMode(relay1,OUTPUT); 
  connectWiFi();
  client_Control.setServer(mqtt_server, mqttPort);
  client_Control.setCallback(mqttSubscriptionCallback);
}

void loop() {
  if (!client_Control.connected()) {
    mqttConnect_Control();
  }
  int soilValue = analogRead(soilPin);
  float temperature1 = dht.readTemperature();
  int temperature = temperature1;
  float filteredMoisture = KalmanFilter(soilValue);
  float soilMoisture1 = (100 - ((filteredMoisture / 4095.00) * 100));
  int soilMoisture = soilMoisture1;
  // Serial.print("Moisture value kalman: ");
  // Serial.print(soilMoisture);
  // Serial.println("%");

  // Serial.print("Temperature: ");
  // Serial.print(temperature);
  // Serial.println(" °C");
  HTTPClient http;
  String url = "https://api.thingspeak.com/update";
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String data_to_send = "api_key=" + String(api_key_write) + "&field1=" + String(temperature) + "&field2=" + String(soilMoisture);
  int httpResponseCode = http.POST(data_to_send);
  if (httpResponseCode > 0) {
    String response = http.getString();
    // Serial.println("Dữ liệu đã được gửi thành công: " + String(data_to_send));
    // Serial.println("HTTP Response code: " + String(httpResponseCode));
    // Serial.println("Server response: " + response);
  } else {
    Serial.print("Lỗi khi gửi dữ liệu. Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  delay(10000);
  client_Control.loop();
}
