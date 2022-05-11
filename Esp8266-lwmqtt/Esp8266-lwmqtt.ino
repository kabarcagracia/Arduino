#include <SPI.h>
#include <MFRC522.h>

/*
In the ESP8266, D3 pin is RST_PIN and
D4 pin is SS_PIN
*/
#define RST_PIN 0
#define SS_PIN 2

MFRC522 reader(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
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

#if defined(ARDUINO_SAMD_MKR1000) or defined(ESP32)
#define __SKIP_ESP8266__
#endif

#if defined(ESP8266)
#define __ESP8266_MQTT__
#endif

#ifdef __SKIP_ESP8266__

#include <Arduino.h>

void setup(){
  Serial.begin(115200);
}

void loop(){
  Serial.println("Hello World");
}

#endif

#ifdef __ESP8266_MQTT__
#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPI.begin();

  reader.PCD_Init();
  // Just wait some seconds...
  delay(4);
  // Prepare the security key for the read and write functions.
  // Normally it is 0xFFFFFFFFFFFF
  // Note: 6 comes from MF_KEY_SIZE in MFRC522.h
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF; //keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
  }
  Serial.println("Ready!");
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

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!reader.PICC_IsNewCardPresent())
  {
    return;
  }

  // Select one of the cards. This returns false if read is not successful; and if that happens, we stop the code
  if (!reader.PICC_ReadCardSerial())
  {
    return;
  }

  // At this point, the serial can be read. We transform from byte to hex

  String serial = "";
  for (int x = 0; x < reader.uid.size; x++)
  {
    // If it is less than 10, we add zero
    if (reader.uid.uidByte[x] < 0x10)
    {
      serial += "0";
    }
    // Transform the byte to hex
    serial += String(reader.uid.uidByte[x], HEX);
    // Add a hypen
    if (x + 1 != reader.uid.size)
    {
      serial += "-";
    }
  }
  // Transform to uppercase
  serial.toUpperCase();

  Serial.println("Read serial is: " + serial);
  String data = "{"
  "\"rfid\":\""+ serial +"\""
  "}";
  publishTelemetry(data);

  // Halt PICC
  reader.PICC_HaltA();
  // Stop encryption on PCD
  reader.PCD_StopCrypto1();
  
  // TODO: Replace with your code here
  /*if (millis() - lastMillis > 60000)
  {
    lastMillis = millis();
    publishTelemetry(getDefaultSensor());
  }*/
}
#endif
