#define TINY_GSM_MODEM_SIM7600
#include <SPI.h>
#include <SD.h>

#include "variables.h"
#include "functions.h"


void setup()
{
    MONITOR.begin(BAUD);
    delay(10);
    MONITOR.println("iniciando");
    if (!SD.begin(SS))
    {
      while (1);
    }

    //INICIAR CON LA SIM APAGADA
    pinMode(POWERSIM7600, OUTPUT);
    digitalWrite(POWERSIM7600, LOW);

    delay(2000);
  
    //INICIAR LED ESP32 ENCENDIDA
    pinMode(LED_ESP32, OUTPUT);
    digitalWrite(LED_ESP32, LOW);
    
    //ENCENDER SIM7600SA
    digitalWrite(POWERSIM7600, HIGH);
  
    //LED ESTADO SIM7600SA
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(500);
    digitalWrite(PWR_PIN, LOW);

    delay(1000);
    //INICIAR SERIAL SIM7600
    SIM7600.begin(BAUD, SERIAL_8N1, RX, TX);

    int retry = 5;
    while (!reply && retry--)
    {
        modem_on();
    }

    if(SendCommand("INICIANDO GPS", "AT+CGPS=1", "OK", "ERROR", 20000))
    {
        MONITOR.println("GPS INICIADO");
    }
    else 
    {
        MONITOR.println("GPS FALLIDO, REINICIANDO");
        digitalWrite(LED_ESP32, LOW);
        ESP.restart();
    }
  
    if(tcp_start())
    {
        MONITOR.println("TCP INICIADO");
        digitalWrite(LED_ESP32, HIGH);
    }
    else {
        MONITOR.println("TCP FALLIDO, REINICIANDO");
        digitalWrite(LED_ESP32, LOW);
        while(!tcp_restart());
    }
}

void loop() {
  //send data since 5 seconds
  if (millis() - send_data > t_send_data)
  {
    send_data = millis();
    publish_data();
  }
  //check datafailed since 10 seconds
  if (millis() - checkSD_data > t_checkSD_data)
  {
    checkSD_data = millis();
    check_SD();
  }
}
