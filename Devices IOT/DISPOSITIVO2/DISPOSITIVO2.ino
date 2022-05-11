char *stringData[] = {
  "\"lat\":-34.4044, \"lng\":-70.87097",
"\"lat\":-34.4045, \"lng\":-70.8706",
"\"lat\":-34.40479, \"lng\":-70.86972",
"\"lat\":-34.40503, \"lng\":-70.86902",
"\"lat\":-34.40518, \"lng\":-70.86849",
"\"lat\":-34.40532, \"lng\":-70.86843",
"\"lat\":-34.40571, \"lng\":-70.8686",
"\"lat\":-34.40598, \"lng\":-70.86875",
"\"lat\":-34.40609, \"lng\":-70.86879",
"\"lat\":-34.40613, \"lng\":-70.86882",
"\"lat\":-34.40623, \"lng\":-70.86872",
"\"lat\":-34.40632, \"lng\":-70.86851",
"\"lat\":-34.40643, \"lng\":-70.86827",
"\"lat\":-34.40655, \"lng\":-70.86794",
"\"lat\":-34.4066, \"lng\":-70.86749",
"\"lat\":-34.40657, \"lng\":-70.86725",
"\"lat\":-34.40677, \"lng\":-70.86662",
"\"lat\":-34.40707, \"lng\":-70.86589",
"\"lat\":-34.40721, \"lng\":-70.86583",
"\"lat\":-34.40729, \"lng\":-70.86541",
"\"lat\":-34.40749, \"lng\":-70.86468",
"\"lat\":-34.40781, \"lng\":-70.86388",
"\"lat\":-34.40813, \"lng\":-70.86292",
"\"lat\":-34.40825, \"lng\":-70.86239",
"\"lat\":-34.40807, \"lng\":-70.86204",
"\"lat\":-34.40708, \"lng\":-70.86167",
"\"lat\":-34.40651, \"lng\":-70.86133",
"\"lat\":-34.40552, \"lng\":-70.86094",
"\"lat\":-34.40472, \"lng\":-70.86038",
"\"lat\":-34.40387, \"lng\":-70.85985",
"\"lat\":-34.40315, \"lng\":-70.85944",
"\"lat\":-34.40145, \"lng\":-70.85796",
"\"lat\":-34.39922, \"lng\":-70.85761",
"\"lat\":-34.3989, \"lng\":-70.85938",
"\"lat\":-34.39857, \"lng\":-70.86225",
"\"lat\":-34.39841, \"lng\":-70.8642",
"\"lat\":-34.39838, \"lng\":-70.8653",
"\"lat\":-34.39798, \"lng\":-70.86643",
"\"lat\":-34.39817, \"lng\":-70.86687",
"\"lat\":-34.39866, \"lng\":-70.86722",
"\"lat\":-34.39897, \"lng\":-70.8674",
"\"lat\":-34.39948, \"lng\":-70.86776",
"\"lat\":-34.39992, \"lng\":-70.86799",
"\"lat\":-34.40026, \"lng\":-70.86835",
"\"lat\":-34.4007, \"lng\":-70.86848",
"\"lat\":-34.40084, \"lng\":-70.8687",
"\"lat\":-34.40123, \"lng\":-70.86892",
"\"lat\":-34.4013, \"lng\":-70.86902",
"\"lat\":-34.40141, \"lng\":-70.8691",
"\"lat\":-34.40153, \"lng\":-70.86912",
"\"lat\":-34.40162, \"lng\":-70.86921",
"\"lat\":-34.40163, \"lng\":-70.86926",
"\"lat\":-34.40175, \"lng\":-70.86934",
"\"lat\":-34.40182, \"lng\":-70.8694",
"\"lat\":-34.40198, \"lng\":-70.86948",
"\"lat\":-34.40194, \"lng\":-70.86981",
"\"lat\":-34.40188, \"lng\":-70.86992",
"\"lat\":-34.4019, \"lng\":-70.87001",
"\"lat\":-34.40188, \"lng\":-70.87011",
"\"lat\":-34.40191, \"lng\":-70.87022",
"\"lat\":-34.40184, \"lng\":-70.87008",
"\"lat\":-34.40173, \"lng\":-70.8701",
"\"lat\":-34.40208, \"lng\":-70.87037",
"\"lat\":-34.40225, \"lng\":-70.87048",
"\"lat\":-34.40247, \"lng\":-70.8706",
"\"lat\":-34.40275, \"lng\":-70.87068",
"\"lat\":-34.40284, \"lng\":-70.87077",
"\"lat\":-34.40291, \"lng\":-70.87083",
"\"lat\":-34.40299, \"lng\":-70.87089",
"\"lat\":-34.40313, \"lng\":-70.87098",
"\"lat\":-34.4033, \"lng\":-70.87106",
"\"lat\":-34.40342, \"lng\":-70.87114",
"\"lat\":-34.40351, \"lng\":-70.8712",
"\"lat\":-34.40368, \"lng\":-70.87114",
"\"lat\":-34.4038, \"lng\":-70.87123",
"\"lat\":-34.40388, \"lng\":-70.87118",
"\"lat\":-34.40395, \"lng\":-70.87114",
"\"lat\":-34.40405, \"lng\":-70.87114",
"\"lat\":-34.40417, \"lng\":-70.87111"
};

int largoArray = 79;
int inicial = 0;
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
    
    Serial.println();
    
    String data = "{" + String(stringData[inicial]) + "," 
      "\"battery\":" + String(bateria(), 1) + ","
      "\"signal\":" + String(WiFi.RSSI()) + ""
    "}";
    if(largoArray - inicial <= 0) {
      inicial = 0;
    }
    Serial.println(data);
    publishTelemetry(data);
    inicial = inicial + 1;
  }
}
