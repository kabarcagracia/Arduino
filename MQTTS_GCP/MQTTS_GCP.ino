
#include <Separador.h>
Separador s;

#include "variables.h"
#include "jwt.h"
#include "simcom.h"
#include "fileManager.h"


#include <ArduinoJson.h>

#include "Wire.h"
#include "ClosedCube_HDC1080.h"
ClosedCube_HDC1080 hdc1080;
#include "RTClib.h"
RTC_DS3231 rtc;


void initRTC(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date &amp; time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date &amp; time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}
long getunixrtc(){
  DateTime now = rtc.now();
  //Full Timestamp
  return now.unixtime();
}
String getRTC(){
  DateTime now = rtc.now();
  String timeUTC = now.timestamp(DateTime::TIMESTAMP_DATE) + " " + now.timestamp(DateTime::TIMESTAMP_TIME) + " UTC";
  return timeUTC;
}
#include <base64.h>
void setup() {
  Serial.begin(115200);
  initRTC();
  hdc1080.begin(0x40);
  initFileManager();
  initModem();
  
  if(downloadCert){
        listDir(SPIFFS, "/", 0);
        delay(10000);
        String caCert = readFile(SPIFFS, "/primbackcert.pem");
        String privateKey = readFile(SPIFFS, "/ecprivate.pem");
    
        if(SendCommand("AT+CCERTDOWN=\"privkey.pem\"," + String(privateKey.length()) , ">", "ERROR", 2000)){
            if(SendCommand(privateKey, "OK", "ERROR", 2000)){
                Serial.println("CLAVE PRIVADA OK");
            } else {
                Serial.println("FAILED");
            }
        } else {
            Serial.println("FAILED");
        }

        if(SendCommand("AT+CCERTDOWN=\"cacert.cert\"," + String(caCert.length()) , ">", "ERROR", 2000)){
            if(SendCommand(caCert, "OK", "ERROR", 2000)){
                Serial.println("CERTIFICADO OK");
            } else {
                Serial.println("FAILED");
            }
        } else {
            Serial.println("FAILED");
        }
    }
  Serial.println("inicializando gps");
  if(SendCommand("AT+CGPS=1", "OK", "ERROR", 5000))
    Serial.println("configurando Access Point Name");
    SendCommand("AT+CGAUTH=1,1,\"" + userApn +"\",\"" + pwdApn + "\"", "OK", "ERROR", 2000);
    SendCommand("AT+CGDCONT=1,\"IP\",\"" + apn + "\",\"0.0.0.0\",0,0", "OK", "ERROR", 2000);
  
    Serial.println("configurando certificados SSL/TLS");
    SendCommand("AT+CSSLCFG=\"sslversion\",0,3", "OK", "ERROR", 2000);
    SendCommand("AT+CSSLCFG=\"authmode\",0,0", "OK", "ERROR", 2000);
    SendCommand("AT+CSSLCFG=\"cacert\",0,\"primbackcert.pem\"", "OK", "ERROR", 2000);
    SendCommand("AT+CSSLCFG=\"clientcert\",0,\"primbackcert.pem\"", "OK", "ERROR", 2000);
    SendCommand("AT+CSSLCFG=\"clientkey\",0,\"privkey.pem\"", "OK", "ERROR", 2000);
    SendCommand("AT+CCHSTART", "OK", "ERROR", 2000);
    SendCommand("AT+CGSOCKCONT=1,\"IP\",\"" + apn + "\"", "OK", "+NETOPEN: 1", 20000);
    
    SendCommand("AT+CSOCKSETPN=1", "OK", "+NETOPEN: 1", 20000);
    SendCommand("AT+CIPMODE=0", "OK", "+NETOPEN: 1", 20000);
    if(SendCommand("AT+NETOPEN?", "+NETOPEN: 0", "+NETOPEN: 1", 20000))
    {
      //desactivada
      if(SendCommand("AT+NETOPEN", "+NETOPEN: 0", "+NETOPEN: 1", 20000))
      {

        //red activada exitosamente
        Serial.println("red lista para iniciar MQTT");
        //red abierta
        SendCommand("AT+IPADDR", "OK", "ERROR", 5000);
    
      } else {
        
        //restablece la esp
        digitalWrite(4, HIGH);
        ESP.restart();
      }
    
    }
    SendCommand("AT+CNTP=\"time1.google.com\"", "OK", "ERROR", 2000);
    SendCommand("AT+CNTP", "ok", "ERROR", 2000);

    if(SendCommand("AT+CMQTTSTART", "0", "ERROR", 20000))
    {
        
        if(SendCommand("AT+CMQTTACCQ=0,\"projects/" + String(project_id) +"/locations/" + String(location) + "/registries/" + String(registry_id) + "/devices/" + String(device_id) + "\",1,4", "OK", "ERROR", 20000))
        {
            if(SendCommand("AT+CMQTTSSLCFG=0,0", "OK", "ERROR", 20000))
            {
                if(SendCommand("AT+CMQTTWILLTOPIC=0," + String(topicinit.length()), ">", "ERROR", 20000))
                {
                       
                        if(SendCommand(topicinit, "OK", "ERROR", 20000))
                        {
                            if(SendCommand("AT+CMQTTWILLMSG=0," + String(msginit.length()) + ",1", ">", "ERROR", 20000))
                            {
                                if(SendCommand(msginit, "OK", "ERROR", 20000))
                                {
                                    if(SendCommand("AT+CMQTTCONNECT=0,\"tcp://" + broker + ":" + port + "\",60,1,\"unused\",\"" + createjwt(getunixrtc(), 60 * 60 * 8
                                    ) + "\"", "+CMQTTCONNECT: 0,0", "ERROR", 20000))
                                    {
                                      if(SendCommand("AT+CMQTTSUBTOPIC=0," + String(topicsubscribe.length()) + ",0", ">", "ERROR", 20000))
                                      {
                                        if(SendCommand(topicsubscribe, "OK", "ERROR", 20000))
                                        {
                                          if(SendCommand("AT+CMQTTSUB=0", "OK", "ERROR", 20000))
                                          {}
                                          
                                        }
                                      }
                                      
                                    }
                                }
                            }
                        }
                    }
                //}
            }
        }
    }

    xTaskCreatePinnedToCore(
    TaskBlink,
    "Taskname",
    2048,
    NULL,
    1,
    NULL,
    1
  );
}

void loop() {
    if (millis() - lastMillis > 3000) {

        //AT+COPS? conocer compañia celular
        //AT+CNSMOD? conocer banda actual
        lastMillis = millis();
        while(publishdata){
            if(flaggpsavailable){
                Serial.println("1er comando");
                Serial1.println("AT+CGPS?");
                flaggpsavailable = false;
            }
            if(flagsignalgprs) {
                Serial1.println("AT+CSQ");
                flagsignalgprs = false; 
            }
            if(flagdatagps){
                Serial.println("2do comando");
                Serial1.println("AT+CGPSINFO");
                flagdatagps = false; 
            }
            if(flag1){
                Serial.println("3er comando");
                Serial1.println("AT+CMQTTTOPIC=0," + String(publishtopic.length()));
                flag1 = false;
            }
            if(flag2){
                Serial.println("4to comando");
                Serial1.println(publishtopic);
                flag2 = false;
            }
            if(flag3){
                Serial.println("5to comando");
                Serial.print("recuperado desde nucleo 1: ");
                Serial.println(extract);
                //LATITUD
                String lat_sxg = s.separa(extract, ',', 0);
                String lat_sxg_h = lat_sxg.substring(0,3);
                String lat_sxg_m = lat_sxg.substring(3);
                String lat_o = s.separa(extract, ',', 1);
                float latitud = lat_sxg_h.toFloat() + (lat_sxg_m.toFloat()/60);
                if(lat_o == "S")
                    latitud = latitud * (-1);
                else if(lat_o == "N")
                    latitud = latitud * (1);

                //LONGITUD
                String lng_sxg = s.separa(extract, ',', 2);
                String lng_sxg_h = lng_sxg.substring(0,3);
                String lng_sxg_m = lng_sxg.substring(3);   
                String lng_o = s.separa(extract, ',', 3);
                float longitud = lng_sxg_h.toFloat() + (lng_sxg_m.toFloat()/60);
                if(lng_o == "W")
                    longitud = longitud * (-1);
              
                else if(lng_o == "E")
                    longitud = longitud * (1);
                
                //VELOCIDAD
                String velocidad = s.separa(extract, ',', 7);
                float vel = velocidad.toFloat() * 1.852;
                payload =
                  "{"
                      "\"type\": \"telemetry\","
                      "\"device_battery\": 70,"
                      "\"device_signal\": " + signal_gprs + ","
                      "\"device_humidity\": " + String(hdc1080.readHumidity())+ ","
                      "\"device_temperature\":" + String(hdc1080.readTemperature(), 2) + ","
                      "\"latitude\":" + String(latitud, 7) + ","
                      "\"longitude\":" + String(longitud, 7) + ","
                      "\"speed_3\":" + String(vel) + ","
                      "\"onoff_1\":\"on\","
                      "\"onoff_2\":\"off\","
                      "\"voltage_4\":12.5,"
                      "\"created_at_origin\": \"" + getRTC() + "\""
                  "}";
                Serial1.println("AT+CMQTTPAYLOAD=0," + String(payload.length()));
                flag3 = false;
            }
            if(flag4){
                Serial.println("6to comando");
                Serial1.println(payload);
                flag4 = false;
            }
            if(flag5){
                Serial.println("7mo comando");
                Serial1.println("AT+CMQTTPUB=0,1,60");
                flag5 = false; 
                break;
            }
        }
    }
}
void TaskBlink(void *pvParameters) {
    bool imprimir = false;
    String response = "";
    for (;;) {
        if(Serial1.available()) {
            response = Serial1.readString();
            imprimir = true;
        }
        if(imprimir) {
            Serial.println("===");
            Serial.println(response);
            Serial.println("===");
            if(response.indexOf(">" >= 0)) {
                if(response.indexOf("AT+CMQTTTOPIC=0,") >= 0) {
                    Serial.println("comando correcto, esperando topic");
                    flag1 = false;
                    flag2 = true;
                } 
                else if(response.indexOf("AT+CMQTTPAYLOAD=0,") >= 0) {
                    flag3 = false;
                    flag4 = true;
                }
              
            }
            if(response.indexOf("OK") >= 0){
                if(response.indexOf(publishtopic) >= 0){
                    flag2 = false;
                    flag3= true;
                } else if(response.indexOf(payload) >= 0){
                    flag4 = false;
                    flag5= true;
                } else if(response.indexOf("+CGPS: 1,1") >= 0) {
                    flagsignalgprs = true;
                } else if(response.indexOf("+CGPS: 0,1") >= 0) {
                    Serial1.println("AT+CGPS=1");
                    flaggpsavailable = true;
                } else if(response.indexOf("+CGPSINFO:") >= 0) {
                    if(response.indexOf("+CGPSINFO: ,,,,,,,,") < 0){
                        extract = s.separa(response, ':', 1);
                        flag1 = true;
                    } else {
                        flagdatagps = true;
                    }
                
                } else if(response.indexOf("+CSQ:") >= 0) {
                  signal_gprs = s.separa(s.separa(response, ':', 1), ',', 0);
                  Serial.println(signal_gprs);
                  flagdatagps = true;
                }
                Serial.println("comando ok");
            }
            if(response.indexOf("ERROR") >= 0) {
                Serial.println("comando erroneo");
            }
            if(response.indexOf("+CMQTTRXSTART") >= 0) {
                String topic = s.separa(response, '\n', 3);
                //Serial.println(topic);
                String msg = s.separa(response, '\n', 5);
                //Serial.println(msg);
                if(msg.length() > 0) {
                    DynamicJsonDocument doc(1024);
                    deserializeJson(doc, msg);

                    int code = doc["code"];
                    String action = doc["action"];
          
                    if(code == 200) {
                        if(action == "setonoff"){
                            String statusCommand = doc["value"];
                            String componentId = doc["componentid"];

                            Serial.print("COMPONENTE RELACIONADO = ");
                            Serial.println(componentId);
                            Serial.print("COMANDO ESTADO = ");
                            Serial.println(statusCommand);
                  
                            if(statusCommand == "on") {
                                //activar rele
                            } else if(statusCommand == "off") {
                                //desactivar rele
                            }
                        }
                        else if(action == "setnewpem"){
                            writeFile(SPIFFS, "/pem.txt", doc["value"]);
                        } 
                        else if(action == "resetdevice"){
                            ESP.restart();
                            //ESP.reset();
                        }
                        else if(action == "settelemetryfrequency"){
                            //define frecuencia de envio de data
                            //ESP.reset();
                        }
                    }
                }
            }
            if(response.indexOf("+CMQTTPUB: 0,") >= 0) {
                flag5 = false;
                flagdatagps = true;
                if(response.indexOf("0,0") >= 0) {
                    Serial.println("envio correcto!!");
                }
                else if(response.indexOf("0,30") >= 0) {
                    ESP.restart();
                }
            }
        }
    response = "";
    imprimir = false;
    delay(10);  
  }
}
