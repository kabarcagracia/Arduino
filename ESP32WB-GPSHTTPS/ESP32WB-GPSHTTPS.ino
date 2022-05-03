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
#include <CloudIoTCoreMqtt.h>

// Cloud iot details.
const char *project_id = "mcd-demos-331211";
const char *location2 = "us-central1";
const char *registry_id = "KRANOS";
const char *device_id = "HNKRNT0002D000002";

const char *private_key_str =
    "e3:0a:b8:a8:1c:41:4b:92:f0:70:7a:aa:38:d1:23:"
    "62:de:a3:50:47:22:cd:cf:8c:54:6f:be:0b:c8:3e:"
    "48:e5";

CloudIoTCoreDevice *device2;

String createJWT(unsigned long iat, int jwt_exp_secs){
    device2 = new CloudIoTCoreDevice(project_id, location2, registry_id, device_id, private_key_str);
    String jwt = device2->createJWT(iat, jwt_exp_secs);
    return jwt;
}
#include <Separador.h>
Separador s;

#include <Wire.h>
#include "ClosedCube_HDC1080.h"

ClosedCube_HDC1080 hdc1080;
#include <base64.h>
bool reply = false;
#define BAUD 115200
#define TX 27
#define RX 26
#define PWR_PIN 4
#define LED_ESP32 12
#define POWERSIM7600 25

String registry = "KRANOS";
String projects = "mcd-demos-331211";
String location = "us-central1";
String device = "HNKRNT0002D000002";
String signal_gprs(){
  

}
String gps_success(){
    bool status_gps = false;
    bool gps = false;
    bool whilegps = true;
    bool cmdgps = true;
    
    while(whilegps) 
    {
        if(cmdgps)
        {
            Serial1.write("AT+CGPSINFO\r");
        }
        cmdgps = false;
        
        while(Serial1.available()) 
        {
            String res = Serial1.readString();
            Serial.println(res);
            if ( res.indexOf("OK") >= 0 ) 
            {
                whilegps = false;
                String e = s.separa(res, ':', 1);

                //LATITUD
                String lat_sxg = s.separa(e, ',', 0);
                String lat_sxg_h = lat_sxg.substring(0,3);
                String lat_sxg_m = lat_sxg.substring(3);
                String lat_o = s.separa(e, ',', 1);
                float latitud = lat_sxg_h.toFloat() + (lat_sxg_m.toFloat()/60);
                if(lat_o == "S")
                {
                    latitud = latitud * (-1);
                }
                else if(lat_o == "N")
                {
                    latitud = latitud * (1);
                }

                //LONGITUD
                String lng_sxg = s.separa(e, ',', 2);
                String lng_sxg_h = lng_sxg.substring(0,3);
                String lng_sxg_m = lng_sxg.substring(3);   
                String lng_o = s.separa(e, ',', 3);
                float longitud = lng_sxg_h.toFloat() + (lng_sxg_m.toFloat()/60);
                if(lng_o == "W")
                {
                    longitud = longitud * (-1);
                }
                else if(lng_o == "E")
                {
                    longitud = longitud * (1);
                }
        
                //FECHA
                String fecha = s.separa(e, ',', 4);
                String dia = fecha.substring(0,2);
                String mes = fecha.substring(2,4);
                String ano = fecha.substring(4,6);
                ano = "20" + ano;
                fecha = ano + "-" + mes + "-" + dia;
          
                //HORA
                String tiempo = s.separa(e, ',', 5);
                String hora = tiempo.substring(0,2);
                String minutos = tiempo.substring(2,4);
                String segundos = tiempo.substring(4,6);
                tiempo = hora + ":" + minutos + ":" + segundos;

                //DATETIME
                String datetime = fecha + " " + tiempo + " UTC"; 
               
                String velocidad = s.separa(e, ',', 7);
                
                //VELOCIDAD
                float vel = velocidad.toFloat() * 1.852;

                Serial.print("T=");
                Serial.print(hdc1080.readTemperature());
                Serial.print("C, RH=");
                Serial.print(hdc1080.readHumidity());
                Serial.println("%");
                delay(3000);
                
                String formatData = 
                "{"
                    "\"type\": \"telemetry\","
                    "\"device_battery\": 70,"
                    "\"device_signal\": 75,"
                    "\"device_humidity\": " + String(hdc1080.readHumidity())+ ","
                    "\"device_temperature\":" + String(hdc1080.readTemperature(), 2) + ","
                    "\"latitude\":" + String(latitud, 7) + ","
                    "\"longitude\":" + String(longitud, 7) + ","
                    "\"speed_1\":" + String(vel) + ","
                    "\"created_at_origin\": \"" + datetime + "\""
                "}";
                if(latitud == 0.000000 || longitud == 0.00000)
                {
                    status_gps = false;
                } else
                {
                    status_gps = true; 
                }

                if(status_gps)
                {
                    return formatData;    
                }
                else
                {
                    return "";
                }
            }
            else if ( res.indexOf("ERROR") >= 0 ) 
            {
                whilegps = false;
                return "";
            }
        }
    }
}
bool SendCommand(String msg, String cmdAT, String resOK, String resERR, int espera){
    bool wait = true;
    bool cmd = true;
    int ti = millis();
    
    while(wait && millis() - ti < espera){
        if(cmd){
            Serial1.println(cmdAT);
        }
        cmd = false;
        while(Serial1.available()){
            String res = Serial1.readString();
          
            Serial.println(res);
            if (res.indexOf(resOK) >= 0 ){
                wait = false;
                return true;
            } else if (res.indexOf(resERR) >= 0 ){
                wait = false;
                return false;
            }
        }
    }
    return false;
}

void modem_on(){
    Serial.println("\nprueba de conexion SIM7600 (10seg aprox.)");
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(300);
    digitalWrite(PWR_PIN, LOW);
    delay(10000);
  
    int i = 10;
    while (i){
        Serial1.println("AT");
        delay(500);
        if (Serial1.available()) {
            String r = Serial1.readString();
            if ( r.indexOf("OK") >= 0 ) {
                reply = true;
                Serial.println("ESTADO OK");
                break;;
            }
        }
        delay(500);
        i--;
    }
}

void setup() {
    Serial.begin(115200);
    initRTC();
    String password = createJWT(getUnixRTC(), 60 * 60 * 8);
    hdc1080.begin(0x40);
    pinMode(25, OUTPUT);
    digitalWrite(25, LOW);
    delay(2000);
    digitalWrite(25, HIGH);
    Serial1.begin(115200, SERIAL_8N1, 26, 27);

    int retry = 5;
    while (!reply && retry--)
    {
        modem_on();
    }
    delay(2000);
    SendCommand("INICIANDO GPS", "AT+CGAUTH=1,1,\"wap\",\"wap\"", "OK", "ERROR", 5000);
    
    SendCommand("INICIANDO GPS", "AT+CGDCONT=1,\"IP\",\"wap.tmovil.cl\",\"0.0.0.0\",0,0", "OK", "ERROR", 5000);
    while(true){
        if(SendCommand("INICIANDO GPS", "AT+CGREG?", "+CGREG: 0,1", "+CGREG: 0,2", 5000))
        {
            break;
        }
        delay(3000);
    }
    Serial.println("registrado en red");

    SendCommand("INICIANDO GPS", "AT+CGPS=1", "OK", "ERROR", 20000);
    if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"sslversion\",0,4", "OK", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"authmode\",0,3", "OK", "ERROR", 20000)) //only server verify
        {
            if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"cacert\",0,\"primbackcert.pem\"", "OK", "ERROR", 20000))
            {
                if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"clientcert\",0,\"primbackcert.pem\"", "OK", "ERROR", 20000))
                {
                    if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"clientkey\",0,\"privkey.pem\"", "OK", "ERROR", 20000))
                    {
                      Serial.println("configuracion SSL correcta!!");
                    }
                }
            }
        }
    }
    SendCommand("INICIANDO GPS", "AT+CGSOCKCONT=1,\"IP\",\"wap.tmovil.cl\"", "OK", "+NETOPEN: 1", 20000);
    
    SendCommand("INICIANDO GPS", "AT+CSOCKSETPN=1", "OK", "+NETOPEN: 1", 20000);
    SendCommand("INICIANDO GPS", "AT+CIPMODE=0", "OK", "+NETOPEN: 1", 20000);
    if(SendCommand("INICIANDO GPS", "AT+NETOPEN?", "+NETOPEN: 0", "+NETOPEN: 1", 20000))
    {
      //desactivada
      if(SendCommand("INICIANDO GPS", "AT+NETOPEN", "+NETOPEN: 0", "+NETOPEN: 1", 20000))
      {

        //red activada exitosamente
        Serial.println("red lista para iniciar MQTT");
        //red abierta
        SendCommand("INICIANDO GPS", "AT+IPADDR", "OK", "ERROR", 5000);
    
      } else {
        
        //restablece la esp
        digitalWrite(PWR_PIN, HIGH);
        ESP.restart();
      }
    
    }
    SendCommand("INICIANDO GPS", "AT+HTTPINIT", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"CONNECTTO\",120", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"RECVTO\",20", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"ACCEPT\",\"*/*\"", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"UA\",\"PostmanRuntime/7.28.4\"", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"SSLCFG\",0", "OK", "ERROR", 20000);
    
    
    
    
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"USERDATA\",\"Authorization: Bearer " + password +"\"", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"URL\",\"https://cloudiotdevice.googleapis.com/v1/projects/mcd-demos-331211/locations/us-central1/registries/KRANOS/devices/HNKRNT0002D000002:publishEvent\"", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"CONTENT\",\"application/json\"", "OK", "ERROR", 20000);
}
void loop(){
  if(SendCommand("INICIANDO GPS", "AT+CGPS?", "OK", "ERROR", 2000)) {
    Serial.println("wena");
    String datadevice = gps_success();
    String encoded = base64::encode(datadevice);
    Serial.println(encoded);
    String json = "{\"binary_data\": \"" + encoded + "\"}";
    if(SendCommand("INICIANDO GPS", "AT+HTTPDATA=" + String(json.length()) +  ",1200", "DOWNLOAD", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", json, "OK", "ERROR", 20000))
        {
          if(SendCommand("INICIANDO GPS", "AT+HTTPACTION=1", "200", "ERROR", 20000))
          {
            
            SendCommand("INICIANDO GPS", "AT+HTTPHEAD", "OK", "ERROR", 20000);
            Serial.println("fin");
          }
        }
    }
    delay(1000);
  }

  
    
}
