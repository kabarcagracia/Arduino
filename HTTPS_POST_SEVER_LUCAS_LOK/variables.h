#define SERVER_IP "http://34.230.28.13:3000/api/gps_send"
#define SERVER_IP2 "http://34.230.28.13:3000/api/gps_multiple_send"
#define STASSID "factorbing"
#define STAPSK  "factorb1989"
#define contador_save_data 10
float latitude = 0.0000000, longitude = 0.0000000, speed_kmh = 0.000, course_deg = 000.000;
int signal_gps = 0;
String datetime = "";

int tiempo_envio = 2000;
int tiempo_envio_multiple = 120000;
static unsigned long lastMillis_sendData = 0;
static unsigned long lastMillis_exit = 0;
static unsigned long lastMillis_saveSD = 0;

static const int RXPin = 4, TXPin = 5;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

Separador s;
