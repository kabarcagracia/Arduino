#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
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
  if (millis() - lastMillis > 60000)
  {
    lastMillis = millis();
    String data = 
    "{"
      "\"ubicacion\": {"
                    "\"lat\": -34.404673,"
                    "\"lng\": -70.8710153,"
                    "\"alt\":  650.2,"
                    "\"sat\": 12"
      "},"
      "\"sensores\": {"
                    "\"tmp_int\": 25.4,"
                    "\"tmp_ext\": 27.9,"
                    "\"hum_int\": 20.1,"
                    "\"lvl_bat\": 97.5,"
                    "\"speed\": 0.00,"
                    "\"signal\": -23.25"
                    "\"estacionado\": True"
      "},"
      "\"time\": \"2021-02-13 21:20:12\""
    "}";
    publishTelemetry(data);
  }
}
