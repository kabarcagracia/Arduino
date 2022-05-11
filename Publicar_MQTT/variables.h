#include <Separador.h>
Separador s;
//seriales
#define MONITOR Serial
#define SIM7600 Serial1
#define BAUD 115200
#define TX 27
#define RX 26
#define PWR_PIN 4
#define LED_ESP32 12
#define POWERSIM7600 25
bool reply = false;

static unsigned long publicar = 0;
int t_publicar = 5000;
//MQTT
String clientMqtt = "factorb0000";
#define serverMqtt "tcp://34.230.28.13"
#define portMqtt 1883
#define username "SIM7600SA"
#define password "SIM7600SAPWD"
#define pubMqtt "/devices"
#define pubConnected "/conectado"
#define msgConnected "conectado"  
