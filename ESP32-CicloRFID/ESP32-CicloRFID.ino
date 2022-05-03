bool statusMachine = false;
String tag = "";
int trans = 0;
String onoff = "off";
String payload = "";
String latitud = "";
String longitud = "";
String velocidad = "";
String satelites = "";
String altitud = "";

#include "leds.h"
#include "mqtt_functions.h"
#include "rfidConf.h"
#include "perifericos.h"

unsigned long lastMillis = 0;
#include <NTPClient.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
void setup() {
    initGPS();
    initRTC();
    initSHT20();
    initLeds();
    Serial.begin(115200);
    initRfid();
    setupCloudIoT();
    timeClient.begin();
}

void loop() {
    mqtt->loop();
    delay(10);

    if (!mqttClient->connected()) {
        connect();
    }
    readRFID();
    if (millis() - lastMillis > 2000) {
        lastMillis = millis();
        int battery = random(0, 100);
        int humidity = random(0, 100);
        int wifi = random(0, 100);
        float temp = random(0, 30);
        timeClient.update();
        getBatInt();
        getGPS();
        payload =
        "{"
            "\"type\": \"telemetry\","
            "\"device_battery\":" + getBatInt() + ","
            "\"device_signal\":" + getSignal() + ","
            "\"device_humidity\":" + getSHT20temp() + ","
            "\"device_temperature\":" + getSHT20hum() + ","
            "\"latitude\": " + latitud + ","
            "\"longitude\": "+ longitud +","
            "\"satellites\": " + satelites + ","
            "\"altitude\": " + altitud +","
            "\"rfid_0\": \"" + tag +"\","
            "\"onoff_1\": \"" + onoff + "\","
            "\"speed_3\": " + velocidad +","
            "\"voltage_4\": " + getBat() + ","
            "\"created_at_origin\": \"" + getRtc() + "\""
        "}";
        //Serial.println(payload);
        publishTelemetry(payload);
    }
}
