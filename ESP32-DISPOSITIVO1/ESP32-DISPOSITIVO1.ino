/*
* Hora UTC [check]
* TEMP y HUM placa interna []
* Intensidad de Señal
* Bateria
* Rfid
* on/off
*/
#include "perifericos.h"
void setup () {
  Serial.begin(115200);
  delay(3000);
  initRTC();
  initSHT20();
}

void loop () {
  Serial.println(getRTC());
  getBAT();
  Serial.println(getSHT20temp());
  Serial.println(getSHT20hum());
   
    Serial.println();
    delay(3000);
}
