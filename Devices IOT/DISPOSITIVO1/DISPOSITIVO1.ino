
#define __ESP8266_MQTT__
#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
  #include <DallasTemperature.h>
#include <OneWire.h>
//configuracion ds18b20
#define ONE_WIRE_BUS 2         
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

#define LED_BUILTIN 13


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  sensors.begin();
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
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
  if (millis() - lastMillis > 5000)
  {
    lastMillis = millis();
    
    String data = 
    "{"
      "\"temp\":" + String(medida(), 1) + ","
      "\"battery\":" + String(bateria(), 1) + ","
      "\"signal\":" + String(WiFi.RSSI()) + ""
    "}";
    Serial.println(data);
    publishTelemetry(data);
  }
}
