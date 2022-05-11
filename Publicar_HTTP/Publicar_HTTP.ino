#define TINY_GSM_MODEM_SIM7600

#include "variables.h"
#include "functions.h"


void setup() {
  MONITOR.begin(BAUD);
  delay(10);

  //ESTADO INICIAL
  pinMode(POWERSIM7600, OUTPUT);
  digitalWrite(POWERSIM7600, LOW);

  delay(3000);
  
  //led ESP32 en LOW enciende
  pinMode(LED_ESP32, OUTPUT);
  digitalWrite(LED_ESP32, LOW);
  //ENCENDER SIM7600SA
  digitalWrite(POWERSIM7600, HIGH);
  //ESTADO SIM7600SA
  //pinMode(PWR_PIN, OUTPUT);
  //digitalWrite(PWR_PIN, HIGH);
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(500);
  digitalWrite(PWR_PIN, LOW);

  delay(1000);

  SIM7600.begin(BAUD, SERIAL_8N1, RX, TX);

  int retry = 5;
  while (!reply && retry--){
    modem_on();
  }

  if(SendCommand("INICIANDO GPS", "AT+CGPS=1", "OK", "ERROR"))
  {
    MONITOR.println("GPS INICIADO");
  }
  else {
    MONITOR.println("GPS FALLIDO, REINICIANDO");
    digitalWrite(LED_ESP32, LOW);
    ESP.restart();
  }
  if(http_start())
  {
    MONITOR.println("HTTP INICIADO");
    digitalWrite(LED_ESP32, HIGH);
  }
  else {
    MONITOR.println("HTTP FALLIDO, REINICIANDO");
    digitalWrite(LED_ESP32, LOW);
    ESP.restart();
  }
}

void loop() {
  //send data since 4 seconds
  if (millis() - send_data > t_send_data)
  {
    send_data = millis();
    digitalWrite(LED_ESP32, LOW);
    publish_data();
    digitalWrite(LED_ESP32, HIGH);
  }
  //check datafailed since 3 minutes
  if (millis() - checkSD_data > t_checkSD_data)
  {
    checkSD_data = millis();
    digitalWrite(LED_ESP32, LOW);
    delay(10);
    digitalWrite(LED_ESP32, HIGH);
    delay(10);
    digitalWrite(LED_ESP32, LOW);
    delay(10);
    digitalWrite(LED_ESP32, HIGH);
    delay(10);
    digitalWrite(LED_ESP32, LOW);
    delay(10);
    digitalWrite(LED_ESP32, HIGH);
    delay(10);
    digitalWrite(LED_ESP32, LOW);
    delay(10);
    check_SD();
    digitalWrite(LED_ESP32, HIGH);
  }
}
