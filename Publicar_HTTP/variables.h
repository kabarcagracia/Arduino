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

static unsigned long send_data = 0;
int t_send_data = 4000; // 4 SEGUNDOS

static unsigned long checkSD_data = 0;
int t_checkSD_data = 1000 * 60 * 3; //3 MINUTOS
