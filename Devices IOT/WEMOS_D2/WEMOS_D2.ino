#include <CloudIoTCore.h>
#define LED_BUILTIN 13
int estado_rele = 0;
const int rele = D2;

//Cliente NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#include "esp8266_mqtt.h"
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
  timeClient.begin();
  timeClient.setTimeOffset(0);
  pinMode(rele, OUTPUT);
  digitalWrite(rele, HIGH);
}

static unsigned long lastMillis = 0;
void loop()
{
  if (!mqtt->loop())
  {
    mqtt->mqttConnect();
  }

  delay(10); // <- fixes some issues with WiFi stability

  // TODO: Replace with your code here
  if (millis() - lastMillis > 10000)
  {
    lastMillis = millis();
    long rand1 = random(0,100);
    long rand2 = random(0,100);
    long rand3 = random(0,100);
    long rand4 = random(0,1);
    String data = 
    "{"
      "\"variables\": {"
          
          "\"random1\":" + String(rand1) + ","
          "\"random2\":" + String(rand2) + ","
          "\"random3\":" + String(rand3) + ","
          "\"random4\":" + String(rand4) + ","
          "\"onoff\":" + String(estado_rele) + ","
          "\"battery_level\": 0.8887897987,"
          "\"signal_level\": 0.84556346436"
      "},"
      "\"created_at_origin\": \"" + String(getTime()) + "\""
    "}";
    Serial.println("publicando data");
    publishTelemetry(data);
  }
}
