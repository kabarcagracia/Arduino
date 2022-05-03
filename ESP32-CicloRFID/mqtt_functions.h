#include <Client.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include "config.h"
#include <ArduinoJson.h>
WiFiClientSecure *netClient;
CloudIoTCoreDevice *device;
CloudIoTCoreMqtt *mqtt;
MQTTClient *mqttClient;
unsigned long iat = 0;
String jwt;

String getJwt(){
    iat = time(nullptr);
    Serial.println("REFRESCANDO JWT");
    jwt = device->createJWT(iat, jwt_exp_secs);
    return jwt;
}

void setupWifi(){
    Serial.println("INICIANDO WIFI");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED){
        delay(100);
    }

    configTime(0, 0, ntp_primary, ntp_secondary);
    Serial.println("SINCRONIZANDO SERVIDOR NTP");
    while (time(nullptr) < 1510644967){
        delay(10);
    }
}

void connectWifi(){
    Serial.print("VERIFICANDO WIFI...");
    while (WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(1000);
    }
}

bool publishTelemetry(String data){
    return mqtt->publishTelemetry(data);
}

void connect(){
    connectWifi();
    mqtt->mqttConnect();
}

void setupCloudIoT(){
    device = new CloudIoTCoreDevice(
        project_id, location, registry_id, device_id,
        private_key_str
    );
  
    setupWifi();
    netClient = new WiFiClientSecure();
    netClient->setCACert(root_cert);
    mqttClient = new MQTTClient(512);
    mqttClient->setOptions(180, true, 1000); // keepAlive, cleanSession, timeout
    mqtt = new CloudIoTCoreMqtt(mqttClient, netClient, device);
    mqtt->setUseLts(true);
    mqtt->startMQTT();
}

void messageReceived(String &topic, String &payload){
    Serial.println("SERVIDOR: " + topic + " - " + payload);
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
  
    if(payload.length() > 0) {

        int code = doc["code"];
        trans = doc["transaction_id"];
        String action = doc["action"];
        String rfidValue = doc["rfidvalue"];
        String statusRele = doc["onoffvalue"];
        String type = doc["type"];
        Serial.println(code);
    
        //sin errores
        if(code == 200 && type == "request") {
            
            if(statusRele == "on") {
                digitalWrite(LED_AZUL, HIGH);
                onoff = "on";
                //statusMachine = true;
      
            } else if(statusRele == "off"){
                digitalWrite(LED_AZUL, LOW);
                onoff = "off";
                //statusMachine = false;
            }
            
            payload = 
            "{"
              "\"type\": \"device_response\","
              "\"action\": \"commandresponse\","
              "\"component_id\": \"rfid_0\","
              "\"currentrfid\": \"" + tag + "\","
              "\"newrfid\": \"" + rfidValue + "\","
              "\"relatedcomponent_id\": \"onoff_1\","
              "\"relatedcomponentvalue\":\"" + onoff + "\","
              "\"transaction_id\": " + trans + ""
            "}";
            publishTelemetry(payload);
            tag = rfidValue;
            Serial.println();
            Serial.println("CONFIRMANDO TRANSACCION");
            Serial.println(payload);
            Serial.println();
            
        }
    }
}
