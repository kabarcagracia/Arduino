//pantalla 20x4
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 4);

//gps ublox
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 13, TXPin = 15;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

//ds18b20
#include <DallasTemperature.h>
#include <OneWire.h>
#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//dht11
#include "DHTesp.h"
#define DHTpin 2
DHTesp dht;

//Cliente NTP
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

void setup() {
    //INICIALIZAR LCD
    lcd.begin();
    lcd.backlight();
    lcd.print(String(device_id));
    lcd.setCursor(0,1); //COLUMNA X FILA
    lcd.print(String(registry_id));
    lcd.setCursor(0,2);
    lcd.print(String(location));
    lcd.setCursor(0,3);
    lcd.print(String(project_id));
    delay(2000);
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Inicializando Nodo");

  
    Serial.begin(115200);
    ss.begin(GPSBaud);
    setupCloudIoT();
    sensors.begin();
    dht.setup(DHTpin, DHTesp::DHT11);
    timeClient.begin();
    timeClient.setTimeOffset(0);
    pinMode(LED_BUILTIN, OUTPUT);
}

static unsigned long lastMillis = 0;
void loop()
{
  if (!mqtt->loop())
  {
    mqtt->mqttConnect();
  }
  delay(10);

  while (ss.available() > 0)
  {
    if (gps.encode(ss.read())){
      digitalWrite(LED_BUILTIN, LOW);
      // TODO: Replace with your code here
      if (millis() - lastMillis > 3000)
      {
        digitalWrite(LED_BUILTIN, HIGH);
        lastMillis = millis();

        sensors.requestTemperatures();

        delay(dht.getMinimumSamplingPeriod());

        float humidity_dht11 = dht.getHumidity();
        float temperature_dht11 = dht.getTemperature();
        
        String create_at_origin = "";
        //validar fecha y hora
        if (gps.time.isValid())
        {
          if (gps.time.hour() < 10) Serial.print(F("0"));
            Serial.print(gps.time.hour());
            Serial.print(F(":"));
          if (gps.time.minute() < 10) Serial.print(F("0"));
            Serial.print(gps.time.minute());
            Serial.print(F(":"));
          if (gps.time.second() < 10) Serial.print(F("0"));
            Serial.print(gps.time.second());
            Serial.print(F("."));
          if (gps.time.centisecond() < 10) Serial.print(F("0"));
            Serial.print(gps.time.centisecond());
          create_at_origin = String(gps.date.year())+"-0"+String(gps.date.month())+"-"+String(gps.date.day())+" "+String(gps.time.hour())+":0"+String(gps.time.minute())+":"+String(gps.time.second());
          Serial.println(create_at_origin);
        }
        else
        {
          Serial.print(F("INVALID"));
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("lt:" + String(gps.location.lat(), 4));
        lcd.setCursor(12,0);
        lcd.print("te:" + String(sensors.getTempCByIndex(0)));
        
        lcd.setCursor(0,1);
        lcd.print("lg:" + String(gps.location.lng(), 4));
        lcd.setCursor(12,1);
        lcd.print("ti:" + String(temperature_dht11, 2));

        
        lcd.setCursor(0,2);
        lcd.print("vel:" + String(gps.speed.kmph()));
        lcd.setCursor(10,2);
        lcd.print("hum:" + String(humidity_dht11, 2));
        
        lcd.setCursor(0,3);
        lcd.print(getTime());
        String data = "{\"temperature\":"+String(sensors.getTempCByIndex(0))+",\"latitude\":"+String(gps.location.lat(), 7)+",\"longitude\":"+String(gps.location.lng(), 7)+",\"speed\":"+String(gps.speed.kmph())+",\"battery_level\": 0.8887897987,\"signal_level\": 0.84556346436,\"plate_temperature\": "+String(temperature_dht11, 3)+",\"created_at_origin\": \""+getTime()+"\"}";
        Serial.println(data);
        publishTelemetry(data);
      }
    }
  }
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("GPS NO DETECTADO");
    while(true);
  }
}
