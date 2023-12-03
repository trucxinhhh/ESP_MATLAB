#include <WiFi.h>  

int GPIO_NUM_2 = 2;

char ssid[] = "forEpic"; //  Change this to your network SSID (name).
char pass[] = "22345698";    // Change this your network password



void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}
void setup() {
  Serial.begin(9600);
  pinMode(GPIO_NUM_2, OUTPUT);
  Serial.print("ok");
  setup_wifi();
}
void loop() {
  
  digitalWrite(GPIO_NUM_2, HIGH);
  delay(3000);
  digitalWrite(GPIO_NUM_2, LOW);
  delay(1000);
}