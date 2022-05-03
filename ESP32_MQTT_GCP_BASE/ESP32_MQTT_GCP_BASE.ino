#include "mqtt_functions.h"
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  setupCloudIoT();
}

void loop() {
  mqtt->loop();
  delay(10);

  if (!mqttClient->connected()) {
    connect();
  }

  if (millis() - lastMillis > 3000) {
    lastMillis = millis();
    Serial.println("Publicando");
    publishTelemetry(getDefaultSensor());
  }
}
