#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>

// Pin definitions
#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ135_PIN 34
#define MQ2_PIN 35

// WiFi credentials
const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

// ThingSpeak API
const char* serverName = "https://api.thingspeak.com/update?api_key=YOUR_API_KEY";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int airQuality = analogRead(MQ135_PIN);
  int gasLevel = analogRead(MQ2_PIN);

  Serial.println("Temp: " + String(temperature) + " C");
  Serial.println("Humidity: " + String(humidity) + " %");
  Serial.println("Air Quality (MQ135): " + String(airQuality));
  Serial.println("Gas Level (MQ2): " + String(gasLevel));

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = serverName + 
                        "&field1=" + String(temperature) + 
                        "&field2=" + String(humidity) + 
                        "&field3=" + String(airQuality) +
                        "&field4=" + String(gasLevel);

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error sending data");
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(15000);
}
