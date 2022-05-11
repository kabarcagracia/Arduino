#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
const uint8_t fingerprint[20] = {0x7e, 0x0b, 0x81, 0x36, 0x4e, 0x63, 0xb3, 0x8a, 0xe2, 0xa5, 0xd6, 0x5b, 0xf5, 0x57, 0xc2, 0x72, 0x14, 0xbf, 0xfc, 0xd4};
const String url_ssl = "https://traklok.kausana.cl/api/restapp/trip_data/";
const String patente_ssl = "KAU123";
const String user_ssl = "gpsplq1";
const String pass_ssl = "p4ss.gps1";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#include "Separador.h"
#include "variables.h"
#include "functions.h"

int cnt = 0;
String archivo = "";
void setup() {

  if (!SD.begin(SS))
  {
    while (1);
  }
  ss.begin(GPSBaud);
  Serial.begin(115200);
  WiFi.begin(STASSID, STAPSK);
  timeClient.begin();
  timeClient.setTimeOffset(0);

}

void loop() {  
    // wait for WiFi connection
    //if ((WiFi.status() == WL_CONNECTED)) {}
      while (ss.available() > 0) 
      {
        if (gps.encode(ss.read())) 
        {
          if(gps.location.lat() == 0.0 && gps.location.lng() == 0.0) 
          {  
            signal_gps = 0;
          } 
          else 
          {
            signal_gps = 1;
          }

          /*Serial.print("Localizacion valida?: ");
          Serial.println(gps.location.isValid());
          Serial.print("Course valida?: ");
          Serial.println(gps.course.isValid());
          Serial.print("Speed valida?: ");
          Serial.println(gps.location.isValid());
          Serial.print("altitud valida?: ");
          Serial.println(gps.altitude.isValid());
        */
          latitude = gps.location.lat();
          longitude = gps.location.lng();
          speed_kmh = gps.speed.kmph();
          course_deg = gps.course.deg();
          
          String dia = format_variable(String(gps.date.day()));
          String mes = format_variable(String(gps.date.month()));
          String ano  = String(gps.date.year());
          String hora  = format_variable(String(gps.time.hour()));
          String minuto = format_variable(String(gps.time.minute()));
          String segundo = format_variable(String(gps.time.second()));
          String file = ano + mes + dia + "_" + hora + minuto + segundo + ".txt";
          datetime = ano + "-" + mes + "-" + dia + "T" + hora + ":" + minuto + ":" + segundo + "Z";

          if(datetime == "2000-00-00T00:00:00Z") {
              datetime = getTimeNtp();  
          }

          
          long rssi = WiFi.RSSI();
          if (millis() - lastMillis_sendData > tiempo_envio) 
          {
            std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
            client->setFingerprint(fingerprint);
            HTTPClient https;
            
            if (https.begin(*client, url)) {
              https.addHeader("Content-Type", "application/json; charset=utf-8;");
              https.addHeader("Username", user);
              https.addHeader("Password", pass);
                String data = 
                  "{"
                      "\"name_device\": \"factorb-iot-0000\", "
                      "\"latitude\": " + String(latitude, 7) + ", "
                      "\"longitude\": " + String(longitude, 7) + ", "
                      "\"battery\": 4.231, "
                      "\"speed\": " + String(speed_kmh, 3) + ", "
                      "\"grade\": " + String(course_deg, 3) + ", "
                      "\"status_gps\": " + String(signal_gps) + ", "
                      "\"wifi\": " + String(rssi) + ", "
                      "\"time\": \"" + datetime + "\""
                  "}";
           
            int httpCode = https.POST(data_f);

            if (httpCode > 0) 
            {
              if (httpCode == HTTP_CODE_OK) 
              {
                const String& payload = http.getString();
              
              }
              
            } 
            else 
            {
            

              if(cnt <= contador_save_data) {
               
                if(cnt == 0){
                  archivo = file;
                }
                Serial.print("\r\nAlmacenado dato numero: ");
                Serial.println(cnt);
                Serial.println(archivo);

                data = 
              "{"
                  "\"latitude\": " + String(latitude, 7) + ", "
                  "\"longitude\": " + String(longitude, 7) + ", "
                  "\"battery\": 4.231, "
                  "\"speed\": " + String(speed_kmh, 3) + ", "
                  "\"grade\": " + String(course_deg, 3) + ", "
                  "\"status_gps\": " + String(signal_gps) + ", "
                  "\"wifi\": " + String(rssi) + ", "
                  "\"time\" : \"" + datetime + "\""
              "}";
                save_SD("/gps/" + archivo, data);
                cnt ++;

              }   
              else {
                cnt = 0;
              }
            }
            http.end();
            lastMillis_sendData = millis();
          }
          

          if(millis() - lastMillis_saveSD > tiempo_envio_multiple) 
          {
            Serial.println("Revisando datos SD");
           
            check_SD();
            
            lastMillis_saveSD = millis();
          }
        } 
      }
} //void loop
