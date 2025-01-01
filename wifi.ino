#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void sendMessage(String message) {
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  HTTPClient http;
  http.begin(url);

  int httpResponseCode = http.GET();

  if (httpResponseCode == 200) {
    Serial.println("Message sent successfully");
  } else {
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}

bool connectWiFi() {
  WiFi.begin(ssid, password);
  Serial.println("\n[+] : Connecting to WiFi...");

  long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startAttemptTime >= 30000) {
      Serial.println("\n[-] : Failed to connect to WiFi within 30 seconds.");
      return false;
    }
    Serial.print(".");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }

  Serial.println("\n[+] : Connected to WiFi");
  tone(4, 1000,500);
  delay(200);
  tone(4, 1000,500);
  delay(200);
  tone(4, 1000,500);
  return true;
}

void disconnectWiFi() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi Disconnected");
}