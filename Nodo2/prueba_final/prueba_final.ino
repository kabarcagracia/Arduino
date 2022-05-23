/******************************************************************************
 * Copyright 2018 Google
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <SoftwareSerial.h>
#include <base64.h>

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupCloudIoT(); // Creates globals for MQTT
  pinMode(LED_BUILTIN, OUTPUT);
  ads.begin();
  
}

static unsigned long lastMillis = 0;
void loop()
{
  int16_t adc0, adc1, adc2, adc3;
  if (!mqtt->loop())
  {
    mqtt->mqttConnect();
  }
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
  delay(100); // <- fixes some issues with WiFi stability
  // This sketch displays information every time a new sentence is correctly encoded.
  
  digitalWrite(LED_BUILTIN, LOW);
  // TODO: Replace with your code here
  if (millis() - lastMillis > 10000)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    lastMillis = millis();          // Send the command to get temperatures  
    Serial.println("Publicando en Google Cloud IoT Core:");
    String data = "{\"ubicacion\": {\"lat\": -33.455347, \"lng\": -70.594446}, \"sensores\": { \"s1\": "+String(adc0)+", \"s2\": "+String(adc1)+", \"s3\": "+String(adc2)+", \"s4\": "+String(adc3)+"}}";
    Serial.println(data);
    String encoded = base64::encode(data); 
    Serial.println(encoded);
    publishTelemetry(encoded);
  }
    
  
}
