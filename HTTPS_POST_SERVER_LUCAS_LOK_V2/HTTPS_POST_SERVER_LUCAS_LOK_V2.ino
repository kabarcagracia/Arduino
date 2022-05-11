#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
const uint8_t fingerprint[20] = {0x7e, 0x0b, 0x81, 0x36, 0x4e, 0x63, 0xb3, 0x8a, 0xe2, 0xa5, 0xd6, 0x5b, 0xf5, 0x57, 0xc2, 0x72, 0x14, 0xbf, 0xfc, 0xd4};
const String url = "https://traklok.kausana.cl/api/restapp/trip_data/";
const String patente = "KAU123";
const String user = "gpsplq1";
const String pass = "p4ss.gps1";
const int data = 4;
ESP8266WiFiMulti WiFiMulti;

//GPS Ublox
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 13, TXPin = 15;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//Cliente NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
void setup() {

  Serial.begin(115200);
  ss.begin(GPSBaud);
  Serial.println();
  Serial.println();
  Serial.println();
  timeClient.begin();
  pinMode(data, INPUT);
  timeClient.setTimeOffset(0);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("iPhone de Kevin", "kevin1234");
}
static unsigned long lastMillis = 0;
static unsigned long lastMillis1 = 0;
void loop() {
  
  if ((WiFiMulti.run() == WL_CONNECTED)) {
       
        if (millis() - lastMillis > 5000) {
          
          digitalWrite(LED_BUILTIN, HIGH);
          lastMillis = millis();
          std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
          client->setFingerprint(fingerprint);
          HTTPClient https;
          
          if (https.begin(*client, url)) {
            https.addHeader("Content-Type", "application/json; charset=utf-8;");
            https.addHeader("Username", user);
            https.addHeader("Password", pass);

            bool ignicion  = digitalRead(data);
            
            String data_f = 
            "{"
              "\"longitud\":-70.2351531,"
              "\"latitud\":-33.2454211,"
              "\"fecha\":\"2021-04-22T00:00:00Z\","
              "\"direccion\":233.3,"
              "\"ignicion\": 0,"
              "\"velocidad\": 12,"
              "\"patente\":\""+patente+"\""
            "}";

            Serial.println(data_f);
            int httpCode = https.POST(data_f);
            
                if (httpCode > 0) {
            
                  Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
      
                  if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                  
                    String payload = https.getString();
                    Serial.println(payload);
                  }
                  else {
                  
                    Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
                  }
                  https.end();
                  
                }
              
             
      }      
    }
  }
}
  
  
  //if ((WiFiMulti.run() == WL_CONNECTED)) {
    
    
    
    /*if (millis() - lastMillis > 5000){
      digitalWrite(LED_BUILTIN, HIGH);
      lastMillis = millis();
      std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
      client->setFingerprint(fingerprint);
      HTTPClient https;
      if (https.begin(*client, url)) {
        Serial.println("Entro al if malparido");
      }
    }  */
    
        
        /*https.addHeader("Content-Type", "application/json; charset=utf-8;");
        https.addHeader("Username", "gpsplq1");
        https.addHeader("Password", "p4ss.gps1");
  
        String data = 
        "{"
          "\"longitud\":-71.3952373,"
          "\"latitud\":-33.0361946,"
          "\"fecha\":\"2020-11-05T15:44:34.193630\","
          "\"direccion\":\"197.0\","
          "\"ignicion\":\"1\","
          "\"velocidad\":2.0,"
          "\"patente\":\""+patente+"\""
        "}";

        int httpCode = https.POST(data);

        if (httpCode > 0) {
      
          Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            
            String payload = https.getString();
            Serial.println(payload);
          }
          else {
            
            Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
          }
          https.end();
        }
      }
      else {
          
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    }*/ //millis
 //void loop



String getTime(){
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  String dia = String(monthDay);
  if(dia.length() == 1)
  {
    dia = "0" + dia;
  }
  int currentMonth = ptm->tm_mon+1;
  String mes = String(currentMonth);
  if(mes.length() == 1)
  {
    mes = "0" + mes;
  }
  int currentYear = ptm->tm_year+1900;
  return String(currentYear)+"-"+mes+"-"+dia+"T"+formattedTime+"Z";
}
