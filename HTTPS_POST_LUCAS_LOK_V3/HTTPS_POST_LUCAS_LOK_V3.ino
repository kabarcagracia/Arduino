#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
const uint8_t fingerprint[20] = {0x7e, 0x0b, 0x81, 0x36, 0x4e, 0x63, 0xb3, 0x8a, 0xe2, 0xa5, 0xd6, 0x5b, 0xf5, 0x57, 0xc2, 0x72, 0x14, 0xbf, 0xfc, 0xd4};
const String url = "https://traklok.kausana.cl/api/restapp/trip_data/";
const String patente = "KAU123";
const String user = "gpsplq1";
const String pass = "p4ss.gps1";

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("factorbing", "factorb1234");
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;

    if (https.begin(*client, url)) {
            https.addHeader("Content-Type", "application/json; charset=utf-8;");
            https.addHeader("Username", user);
            https.addHeader("Password", pass);


            String data_f = 
            "{"
              "\"longitud\": -70.25648984,"
              "\"latitud\": -33.4658464651,"
              "\"fecha\":\"2021-04-22T18:17:15\","
              "\"direccion\": 125.2,"
              "\"ignicion\": 0,"
              "\"velocidad\": 10,"
              "\"patente\":\""+patente+"\""
            "}";

            Serial.println(data_f);
            int httpCode = https.POST(data_f);

            if (httpCode > 0) {
      
            Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            
              String payload = https.getString();
              Serial.println(payload);
            }
            else {
            
              Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
            }
            https.end();
          }
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  Serial.println("Wait 10s before next round...");
  delay(10000);
}
