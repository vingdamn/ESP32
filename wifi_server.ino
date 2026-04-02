#include <WiFi.h>

const char* ap_ssid = "ESP32_AP";
const char* ap_password = "12345678";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.printf("AP Started: %s\n", WiFi.softAPIP().toString().c_str());
}

void loop() {
  static int last_clients = 0;
  int current_clients = WiFi.softAPgetStationNum();
  
  if (current_clients != last_clients) {
    last_clients = current_clients;
    if (current_clients > 0) {
      Serial.println("Client connected");
    } else {
      Serial.println("Client disconnected");
    }
  }
  
  delay(00);
}