#include <ESP8266WiFi.h>
#include "FS.h"

#include "WiFiClientSecureBearSSL.h"
#include <time.h>

#include <MQTT.h>

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include "ciotc_config.h"

void messageReceivedAdvanced(MQTTClient *client, char topic[], char bytes[], int length)
{
  if (length > 0){
    Serial.printf("incoming: %s - %s\n", topic, bytes);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("tpc: "+String(topic));
    lcd.setCursor(0,2);
    lcd.print("cmd: "+String(bytes));
    delay(500);
    

    
  } else {
    Serial.printf("0\n"); // Success but no message
  }
}
///////////////////////////////

// Initialize WiFi and MQTT for this board
static MQTTClient *mqttClient;
static BearSSL::WiFiClientSecure netClient;
static BearSSL::X509List certList;
static CloudIoTCoreDevice device(project_id, location, registry_id, device_id);
CloudIoTCoreMqtt *mqtt;

///////////////////////////////
// Helpers specific to this board
///////////////////////////////
String getDefaultSensor()
{
  return "Wifi: " + String(WiFi.RSSI()) + "db";
}

String getJwt()
{
  // Disable software watchdog as these operations can take a while.
  ESP.wdtDisable();
  time_t iat = time(nullptr);
  lcd.clear();
  lcd.setCursor(3,1);
  lcd.print("GENERANDO JWT.");
  lcd.setCursor(2,3);
  lcd.print("tiempo: "+String(jwt_exp_secs)+" seg");
  Serial.println("Refreshing JWT");
  String jwt = device.createJWT(iat, jwt_exp_secs);
  ESP.wdtEnable(0);
  return jwt;
}

static void readDerCert(const char *filename) {
  File ca = SPIFFS.open(filename, "r");
  if (ca)
  {
    size_t size = ca.size();
    uint8_t cert[size];
    ca.read(cert, size);
    certList.append(cert, size);
    ca.close();

    Serial.print("Success to open ca file ");
  }
  else
  {
    Serial.print("Failed to open ca file ");
  }
  Serial.println(filename);
}

static void setupCertAndPrivateKey()
{
  // Set CA cert on wifi client
  // If using a static (pem) cert, uncomment in ciotc_config.h:
  certList.append(primary_ca);
  certList.append(backup_ca);
  netClient.setTrustAnchors(&certList);

  device.setPrivateKey(private_key);
  return;

  // If using the (preferred) method with the cert and private key in /data (SPIFFS)
  // To get the private key run
  // openssl ec -in <private-key.pem> -outform DER -out private-key.der

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  readDerCert("/gtsltsr.crt"); // primary_ca.pem
  readDerCert("/GSR4.crt"); // backup_ca.pem
  netClient.setTrustAnchors(&certList);


  File f = SPIFFS.open("/private-key.der", "r");
  if (f) {
    size_t size = f.size();
    uint8_t data[size];
    f.read(data, size);
    f.close();

    BearSSL::PrivateKey pk(data, size);
    device.setPrivateKey(pk.getEC()->x);

    Serial.println("Success to open private-key.der");
  } else {
    Serial.println("Failed to open private-key.der");
  }

  SPIFFS.end();
}

static void setupWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Conectando Wi-fi");
  lcd.setCursor(0,3);
  lcd.print("SSID: " + String(ssid));
  
  
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if(i < 60){
      lcd.setCursor(5,1);
      lcd.print(String(i) + " segundos");
      i = i + 1;
    }
    else {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Error, presione RST.");
    }
    delay(1000);
    
  }
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Wi-fi Conectado.");
  lcd.setCursor(1,3);
  lcd.print("Sincronizando NTP.");
  configTime(0, 0, ntp_primary, ntp_secondary);
  Serial.println("Waiting on time sync...");
  while (time(nullptr) < 1510644967)
  {
    delay(10);
  }
}

///////////////////////////////
// Orchestrates various methods from preceeding code.
///////////////////////////////
bool publishTelemetry(const String &binary_data)
{
  return mqtt->publishTelemetry(binary_data);
}

bool publishTelemetry(const char *binary_data, int length)
{
  return mqtt->publishTelemetry(binary_data, length);
}

// TODO: fix globals
void setupCloudIoT()
{
  // ESP8266 WiFi setup
  setupWifi();

  // ESP8266 WiFi secure initialization and device private key
  setupCertAndPrivateKey();

  mqttClient = new MQTTClient(512);
  mqttClient->setOptions(180, true, 1000); // keepAlive, cleanSession, timeout
  mqtt = new CloudIoTCoreMqtt(mqttClient, &netClient, &device);
  mqtt->setUseLts(true);
  mqtt->startMQTTAdvanced(); // Opens connection using advanced callback
}


String getTime(){
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  String dia = String(monthDay);
  if(dia.length() == 1)
  {
    dia = "0" + dia;
  }
  int currentMonth = ptm->tm_mon+1;
  String mes = String(currentMonth);
  if(mes.length() == 1)
  {
    mes = "0" + mes;
  }
  int currentYear = ptm->tm_year+1900;
  return String(currentYear)+"-"+mes+"-"+dia+" "+formattedTime;
}
