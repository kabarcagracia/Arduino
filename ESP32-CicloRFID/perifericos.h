#include "Wire.h"
#include "SHT2x.h"
#include "RTClib.h"

RTC_DS3231 rtc;
SHT2x sht;

#include <TinyGPS++.h>
TinyGPSPlus gps;
#define RXD2 16
#define TXD2 17
#define BAUDRATE 9600

uint32_t start;
uint32_t stop;

const int pinADC = 36;
const int pinADCInt = 39;
int Adc = 0;
float voltajeAdc = 0.0;
float voltajeBat = 0.0;
int AdcInt = 0;
float voltajeAdcInt = 0.0;
float voltajeBatInt = 0.0;
long mapa(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void initGPS(){
    Serial2.begin(BAUDRATE, SERIAL_8N1, RXD2, TXD2);
}
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
void initSHT20(){
    Serial.println(__FILE__);
    Serial.print("SHT2x_LIB_VERSION: \t");
    Serial.println(SHT2x_LIB_VERSION);
  
    sht.begin();
  
    uint8_t stat = sht.getStatus();
    Serial.print(stat, HEX);
    Serial.println();
}
String getSignal(){
  int wifi = WiFi.RSSI();
  long val = mapa(wifi, -120, -50, 0, 100);
  Serial.print("wifi [dbm]: ");
  Serial.println(wifi);
  Serial.print("% señal: ");
  Serial.println(val);
  return String(val);
}
String getRtc(){
  DateTime now = rtc.now();

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
String getSHT20temp(){
  start = micros();
  sht.read();
  stop = micros();

  return String(sht.getTemperature(), 2);
}
String getSHT20hum(){
  start = micros();
  sht.read();
  stop = micros();

  return String(sht.getHumidity(), 2);
}
String getBat(){
    Adc = analogRead(pinADC);
    voltajeAdc = (Adc * 3.3)/(4095); 
    voltajeBat = voltajeAdc / ( 1.2/( 1.2 + 10.0));
    return String(voltajeBat, 2);
}
String getBatInt(){
    AdcInt = analogRead(pinADCInt);
    voltajeAdcInt = (AdcInt * 3.3)/(4095); 
    voltajeBatInt = voltajeAdcInt / ( 1.7/( 1.0 + 1.7));
    int porcent = (voltajeBatInt * 100)/4.2;


    return String(porcent);
}
void displayInfo()
{
  //Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    latitud = String(gps.location.lat(), 6);
    longitud = String(gps.location.lng(), 6);
    //Serial.print(gps.location.lat(), 6);
    //Serial.print(F(","));
    //Serial.print(gps.location.lng(), 6);
  }
  else
  {
    //Serial.print(F("INVALID"));
  }

  //Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    //Serial.print(gps.date.month());
    //Serial.print(F("-"));
    //Serial.print(gps.date.day());
    //Serial.print(F("-"));
    //Serial.print(gps.date.year());
  }
  else
  {
    //Serial.print(F("INVALID"));
  }

  //Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    //Serial.print(gps.time.hour());
    //Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    //Serial.print(gps.time.minute());
    //Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    //Serial.print(gps.time.second());
    //Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    //Serial.print(gps.time.centisecond());
  }
  else
  {
    //Serial.print(F("INVALID"));
  }
  velocidad = String(gps.speed.kmph(), 2);
  altitud = String(gps.altitude.meters(), 2);
  satelites = String(gps.satellites.value());
  /*Serial.print(" Speed: ");
  Serial.print(gps.speed.kmph());
  Serial.print(" Course: ");
  Serial.print(gps.course.deg());
  Serial.print(" Altitude: ");
  Serial.print(gps.altitude.meters());
  Serial.print(" Satellites:");
  Serial.print(gps.satellites.value());
  Serial.println();*/
  
}
void getGPS(){
    while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
        displayInfo();

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        while(true);
    }
}
