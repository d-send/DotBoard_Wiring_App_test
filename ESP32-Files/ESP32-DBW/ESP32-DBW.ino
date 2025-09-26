#include <WiFi.h>

const char* ssid = "SLT-Fiber-7533";
const char* password = "145@7533";

WiFiServer server(1234);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected!");
    while (client.connected()) {
      if (client.available()) {
        String msg = client.readStringUntil('\n');
        if (msg.length() > 0) {
          Serial.println("Received: " + msg);
        }
      }
    }
    Serial.println("Client disconnected.");
  }
}