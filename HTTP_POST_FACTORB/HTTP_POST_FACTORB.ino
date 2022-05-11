#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
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
          
          
          if (millis() - lastMillis_sendData > tiempo_envio) 
          {
            if(gps.location.lat() == 0.0 && gps.location.lng() == 0.0) 
            {  
              signal_gps = 0;
            } 
            else 
            {
              signal_gps = 1;
            }
            long rssi = WiFi.RSSI();
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
            if(signal_gps == 1){
            WiFiClient client;
            HTTPClient http;     
            http.setTimeout(2000);
            http.begin(client, SERVER_IP);
            http.addHeader("Content-Type", "application/json");
  
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
           
            int httpCode = http.POST(data);

            if (httpCode > 0) 
            {
              if (httpCode == HTTP_CODE_OK) 
              {
                const String& payload = http.getString();
              
              }
              
            } 
            else 
            {
            
                Serial.print("\r\nAlmacenado dato numero: ");
                Serial.println(cnt);
                Serial.println(file);
                save_SD("/gps/" + file, data);

            }
            
            http.end();
            lastMillis_sendData = millis();
            }
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
