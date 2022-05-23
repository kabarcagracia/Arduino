//DHT11
#include "DHTesp.h"
#define DHTpin D4
DHTesp dht;

//Cliente NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const int rele = D2;
#define LED_BUILTIN 13

#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

void setup() {
    Serial.begin(115200);
    setupCloudIoT();
    timeClient.begin();
    timeClient.setTimeOffset(0);
    dht.setup(DHTpin, DHTesp::DHT11);
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
