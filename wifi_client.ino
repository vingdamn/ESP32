#include <WiFi.h>

// WiFi credentials
const char* ssid = "ESP32_AP";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
  // put your main code here, to run repeatedly:

}
