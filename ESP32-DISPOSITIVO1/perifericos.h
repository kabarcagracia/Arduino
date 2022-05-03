#include "Wire.h"
#include "SHT2x.h"
#include "RTClib.h"

RTC_DS3231 rtc;
SHT2x sht;

uint32_t start;
uint32_t stop;

const int pinADC = 36;
int Adc = 0;
float voltajeAdc = 0.0;
float voltajeBat = 0.0;

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
String getRTC(){
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

String getBAT(){
  Adc = analogRead(pinADC);

  voltajeAdc = (Adc * 3.3)/(4095);

  voltajeBat = voltajeAdc / ( 1.2/( 1.2 + 10.0));

  Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
  Serial.print("ADC: ");
  Serial.print(Adc);
  Serial.print("\tADC [V]: ");
  Serial.print(voltajeAdc);
  Serial.print("\tBAT [V]: ");
  Serial.println(voltajeBat);
  Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");

  return String(voltajeBat, 2);
}
