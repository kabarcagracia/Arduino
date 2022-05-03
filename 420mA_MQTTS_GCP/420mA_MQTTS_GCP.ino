#include "Wire.h"
#include <Separador.h>
Separador s;

#include "rtc.h"
#include "hdc1080.h"
#include "filemanager.h"
#include "lcd.h"
#include "enviroments.h"
#include "simcom.h"
#include "jwt.h"

void setup(){
    Serial.begin(115200);
    Serial.println("");
    Serial.println("HEXANODE - SIMCOM7600SA");
    Serial.println("");
    initFileManager();
    initrtc();
    inithdc();

    Serial.println(getunixrtc());
    Serial.println(getutcrtc());
    Serial.println(gettemp());
    Serial.println(gethum());
    
    initmodem();
    downloadcert();
    initgps();
    configapn();
    setssl();
    opennetwork();
    configmqtt();
    statusmqtt = connectmqtt(createjwt(getunixrtc(), 60 * 4));
}

void loop(){
  if(statusmqtt == "success") {
    if(SendCommand("AT+CGPS?", "OK", "ERROR", 5000)) {
      //String datadevice = gps_success();
      String datadevice =
        "{"
          "\"type\": \"telemetry\","
          "\"device_battery\": 70,"
          "\"device_signal\": 75,"
          "\"device_humidity\": 12,"
          "\"device_temperature\": 12,"
          "\"latitude\": -33.45687,"
          "\"longitude\": -70.456876,"
          "\"onoff_1\": \"off\","
          "\"onoff_2\": \"on\","
          "\"speed_3\": 0,"
          "\"voltage_4\": 12.5,"
          "\"created_at_origin\": \"2021-12-13 19:37:12 UTC\""
        "}";
      if(datadevice != ""){
        SendCommand("AT+CMQTTTOPIC=0," + String(topicpublish.length()), ">", "ERROR", 20000);
        SendCommand(topicpublish, "OK", "0", 20000);
        SendCommand("AT+CMQTTPAYLOAD=0," + String(datadevice.length()), ">", "ERROR", 20000);
        SendCommand(datadevice, "OK", "0", 20000);
        SendCommand("AT+CMQTTPUB=0,1,60", "OK", "ERROR", 20000);
      }
    }
    
  } else if(statusmqtt == "password") {
      
      Serial.println("actualizando jwt"); 
  } else {
      
      Serial.println(statusmqtt);
      delay(10000);
  }
    
}
