#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

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
    medida();
    Serial.println(medida());

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
