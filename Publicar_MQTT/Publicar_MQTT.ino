#define TINY_GSM_MODEM_SIM7600

#include "variables.h"
#include "functions.h"


void setup() {
  MONITOR.begin(BAUD);
  delay(10);

  //ESTADO INICIAL
  pinMode(POWERSIM7600, OUTPUT);
  digitalWrite(POWERSIM7600, LOW);

  delay(2000);
  
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
  if(mqtt_start())
  {
    MONITOR.println("MQTT EXITOSO");
    digitalWrite(LED_ESP32, HIGH);
  }
  else {
    MONITOR.println("MQTT FALLIDO");
    digitalWrite(LED_ESP32, LOW);
    ESP.restart();
  }
}

void loop() {
  if (millis() - publicar > t_publicar)
  {
    publicar = millis();
    String datadevice = gps_success();
    MONITOR.println(datadevice);
    if(datadevice != ""){
      
      digitalWrite(LED_ESP32, LOW);
      
      publicar_data(datadevice);
      digitalWrite(LED_ESP32, HIGH);
    }
    
  }
  
  /*  while (MONITOR.available()) {
        SIM7600.write(MONITOR.read());
    }*/
}
