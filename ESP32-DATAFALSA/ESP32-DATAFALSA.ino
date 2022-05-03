#include "mqtt_functions.h"
unsigned long lastMillis = 0;
#include <NTPClient.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  setupCloudIoT();
  timeClient.begin();
}

void loop() {
  mqtt->loop();
  delay(10);

  if (!mqttClient->connected()) {
    connect();
  }

  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    int battery = random(0, 100);
    int humidity = random(0, 100);
    int wifi = random(0, 100);
    float temp = random(0, 30);
    //timeClient.update();
    String payload =
      "{"
        "\"rfid_0\": \"\","
        "\"onoff_1\": \"on\","
        "\"latitude_2\": -34.58948494859485,"
        "\"longitude_2\": -70.58948494859485,"
        "\"satellites_2\": 5,"
        "\"altitude_2\": 20,"
        "\"voltage_3\": 20.0,"
        "\"current_4\": 20.0,"
        "\"temperature_5\": 34.3,"
        "\"device_battery\": 100,"
        "\"device_signal\": 90,"
        "\"device_temperature\": 23,"
        "\"device_humidity\": 20,"
        "\"created_at_origin\": \"2022-01-06 14:50:30 UTC\""
      "}";
      "}";
      
    Serial.println("Publicando");
    Serial.println(payload);
    publishTelemetry(payload);
  }
}
