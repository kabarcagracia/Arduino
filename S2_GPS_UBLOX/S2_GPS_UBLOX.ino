//Cliente NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
//configuracion gps
static const int RXPin = 13, TXPin = 15;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

const int rele = D2;
#define LED_BUILTIN 13

#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

void setup() {
    Serial.begin(115200);
    ss.begin(9600);
    setupCloudIoT();
    timeClient.begin();
    timeClient.setTimeOffset(0);
    pinMode(rele, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

static unsigned long lastMillis = 0;
void loop()
{
  if (!mqtt->loop())
  {
    mqtt->mqttConnect();
  }
  
  delay(10);
  
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      digitalWrite(LED_BUILTIN, LOW);
      
      if (millis() - lastMillis > 3000)
      {
        digitalWrite(LED_BUILTIN, HIGH);
        lastMillis = millis();
        
        String data = "{\"ubicacion\": {\"lat\": "+String(gps.location.lat(), 7)+", \"lng\": "+String(gps.location.lng(), 7)+", \"alt\": "+String(gps.altitude.meters())+", \"date\": "+String(gps.date.value())+", \"time\": "+String(gps.time.value())+", \"speed\": "+String(gps.speed.kmph())+", \"gps_uso\": "+String(gps.satellites.value())+", \"status\": "+String(estado)+"}, \"sensores\": { \"s1\": "+String(t)+", \"s2\": "+String(h)+", \"s3\": "+String(-1)+", \"s4\": "+String(-1)+"}}";
        publishTelemetry(data);
      }
    }
  }
    
        

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }


  if (millis() - lastMillis > 3000)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    lastMillis = millis();

    delay(dht.getMinimumSamplingPeriod());

    float humidity_dht11 = dht.getHumidity();
    float temperature_dht11 = dht.getTemperature();
        
   
   
    String data = 
    "{"
      "\"temperatura\":" + String(temperature_dht11, 3) + ","
      "\"humedad\":" + String(humidity_dht11, 3) + ","
      "\"rele_000\":" + String(rele_status) + ","
      "\"battery_level\": 0.8887897987,"
      "\"signal_level\": 0.84556346436,"
      "\"created_at_origin\": \"" + getTime() + "\""
    "}";
    
    Serial.println(data);
    publishTelemetry(data);
  }
}
