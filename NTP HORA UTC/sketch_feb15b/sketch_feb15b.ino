#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char *ssid     = "VALENTINA";
const char *password = "1197val200826";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(0);
}

void loop() {
  timeClient.update();
  String formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);
  unsigned long epochTime = timeClient.getEpochTime();
  Serial.println(epochTime);
  String formattedTime = timeClient.getFormattedTime();
  Serial.println(formattedTime);
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
  Serial.println(String(currentYear)+"-"+mes+"-"+dia+" "+formattedTime);

  delay(2000);

}
