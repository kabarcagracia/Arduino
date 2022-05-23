#include <CloudIoTCore.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

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
  pinMode(RELE, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(setpoint, OUTPUT);
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
  timeClient.begin();
  timeClient.setTimeOffset(0);
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
    Serial.print(medida());
    Serial.println(" -- "+getTime());

    //aqui va el objeto
    
    publishTelemetry(getDefaultSensor());
  }


  if(medida() > 10){
      digitalWrite(setpoint, HIGH);
  }
  else {
    digitalWrite(setpoint, LOW);
  }
}
