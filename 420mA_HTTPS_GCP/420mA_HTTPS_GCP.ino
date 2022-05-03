/*
  Codigo conversor 4-20mA y HTTPS iot core.
*/
#include "Wire.h"

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
long getUnixRTC(){
  DateTime now = rtc.now();
  //Full Timestamp
  return now.unixtime();
}
String getRTC(){
  DateTime now = rtc.now();
  //Full Timestamp
  Serial.println(now.unixtime());
 Serial.println(String("DateTime::TIMESTAMP_FULL:\t")+now.timestamp(DateTime::TIMESTAMP_FULL));

 //Date Only
 Serial.println(String("DateTime::TIMESTAMP_DATE:\t")+now.timestamp(DateTime::TIMESTAMP_DATE));

 //Full Timestamp
 Serial.println(String("DateTime::TIMESTAMP_TIME:\t")+now.timestamp(DateTime::TIMESTAMP_TIME));
  String yearNow = String(now.year());
  String monthNow = String(now.month());
  String dayNow = String(now.day());
  String hourNow = String(now.hour());
  String minuteNow = String(now.minute());
  String secondNow = String(now.second());

  if(monthNow.length() == 1) monthNow = "0" + monthNow;
  if(dayNow.length() == 1) dayNow = "0" + dayNow;
  if(hourNow.length() == 1) hourNow = "0" + hourNow;
  if(minuteNow.length() == 1) minuteNow = "0" + minuteNow;
  if(secondNow.length() == 1) secondNow = "0" + secondNow;



    String timeUTC = 
      yearNow + "-" + 
      monthNow + "-" + 
      dayNow + " " +  
      hourNow + ":" + 
      minuteNow+ ":" +
      secondNow + " UTC";
   return timeUTC;
}
#include <base64.h>
#include "FS.h"
#include "SPIFFS.h"

#include "variables.h"
#include "jwt.h"
#include "simcom.h"
#include "fileManager.h"

#include <Separador.h>
Separador s;

void setup() {
    Serial.begin(115200);
    initRTC();
    initFileManager();
    initModem();

    if(downloadCert){
        listDir(SPIFFS, "/", 0);
        String caCert = readFile(SPIFFS, "/primbackcert.pem");
        String privateKey = readFile(SPIFFS, "/HNTSTT0005D000005.pem");
    
        if(SendCommand("AT+CCERTDOWN=\"privkey.pem\"," + String(privateKey.length()) , ">", "ERROR", 2000)){
            if(SendCommand(privateKey, "OK", "ERROR", 2000)){
                Serial.println("CLAVE PRIVADA OK");
            } else {
                Serial.println("FAILED");
            }
        } else {
            Serial.println("FAILED");
        }

        if(SendCommand("AT+CCERTDOWN=\"cacert.pem\"," + String(caCert.length()) , ">", "ERROR", 2000)){
            if(SendCommand(caCert, "OK", "ERROR", 2000)){
                Serial.println("CERTIFICADO OK");
            } else {
                Serial.println("FAILED");
            }
        } else {
            Serial.println("FAILED");
        }
    }
  
    Serial.println("configurando Access Point Name");

    if(){}
    SendCommand("AT+CGAUTH=1,1,\"" + userApn +"\",\"" + pwdApn + "\"", "OK", "ERROR", 2000);
    SendCommand("AT+CGDCONT=1,\"IP\",\"" + apn + "\",\"0.0.0.0\",0,0", "OK", "ERROR", 2000);
  
    Serial.println("configurando certificados SSL/TLS");
    SendCommand("AT+CSSLCFG=\"sslversion\",0,4", "OK", "ERROR", 2000);
    SendCommand("AT+CSSLCFG=\"authmode\",0,3", "OK", "ERROR", 2000);
    SendCommand("AT+CSSLCFG=\"cacert\",0,\"cacert.pem\"", "OK", "ERROR", 2000);
    SendCommand("AT+CSSLCFG=\"clientcert\",0,\"cacert.pem\"", "OK", "ERROR", 2000);
    SendCommand("AT+CSSLCFG=\"clientkey\",0,\"privkey.pem\"", "OK", "ERROR", 2000);

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
    
    SendCommand("AT+HTTPINIT", "OK", "ERROR", 2000);
    SendCommand("AT+HTTPPARA=\"CONNECTTO\",120", "OK", "ERROR", 2000);
    SendCommand("AT+HTTPPARA=\"RECVTO\",20", "OK", "ERROR", 2000);
    SendCommand("AT+HTTPPARA=\"ACCEPT\",\"*/*\"", "OK", "ERROR", 2000);
    SendCommand("AT+HTTPPARA=\"UA\",\"ESP32-WB\"", "OK", "ERROR", 2000);
    SendCommand("AT+HTTPPARA=\"SSLCFG\",0", "OK", "ERROR", 2000);
    
    SendCommand("AT+HTTPPARA=\"USERDATA\",\"Authorization: Bearer " + createJWT(getUnixRTC(), 60 * 60 * 8) +"\"", "OK", "ERROR", 2000);
    SendCommand("AT+HTTPPARA=\"URL\",\"https://cloudiotdevice.googleapis.com/v1/projects/" + String(project_id) +"/locations/" + String(location) + "/registries/" + String(registry_id) + "/devices/" + String(device_id) + ":publishEvent\"", "OK", "ERROR", 2000);
    SendCommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"", "OK", "ERROR", 20000);
    //SendCommand("AT+CCLK=\"21/12/07,19:45:00+00\"", "ok", "error", 2000);
  Serial.println(createJWT(1638901966, 1200));

}

void loop() {
  getRTC();
  String atcommand = SendCommand2("AT+CCLK?", "ok", "error", 2000);
  Serial.println("fecha");
  Serial.println(atcommand);
  String e = s.separa(atcommand, '\n', 1);
  Serial.println(e);
  String f = s.separa(e, '"', 1);
  Serial.println(f);
  String g = s.separa(f, ',', 0);
  String h = s.separa(f, ',', 1);
  Serial.println("20" + g);
  String i  = s.separa(h, '+', 0);
  Serial.println(i);
  if(true) {
    Serial.println("wena");
    String datadevice = "{\"type\": \"telemetry\", \"voltage_0\": 23.5}";
    String encoded = base64::encode(datadevice);
    Serial.println(encoded);
    String json = "{\"binary_data\": \"" + encoded + "\"}";
    if(SendCommand("AT+HTTPDATA=" + String(json.length()) +  ",1200", "DOWNLOAD", "ERROR", 20000))
    {
        if(SendCommand(json, "OK", "ERROR", 20000))
        {
          if(SendCommand("AT+HTTPACTION=1", "200", "ERROR", 20000))
          {
            
            SendCommand("AT+HTTPHEAD", "OK", "ERROR", 20000);
            Serial.println("fin");
          }
        }
    }
    delay(1000);
  }

  
    

}
