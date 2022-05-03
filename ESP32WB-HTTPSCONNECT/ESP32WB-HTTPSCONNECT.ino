bool reply = false;
#define BAUD 115200
#define TX 27
#define RX 26
#define PWR_PIN 4
#define LED_ESP32 12
#define POWERSIM7600 25

String registry = "TEST";
String projects = "mcd-demos-331211";
String location = "us-central1";
String device = "HNTSTT0002D000002";
String password = "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE2Mzc5NTYxMzEsImV4cCI6MTYzNzk3MDUzMSwiYXVkIjoibWNkLWRlbW9zLTMzMTIxMSJ9.jswkm-Ctgm0d5YFsganS7bJ2gmTOxvZIMxM4J3IAy9gUFzsiQLcLCTPUv-NIMUv3E-VzPfQS-XbWs4BkYavRdA";

bool SendCommand(String msg, String cmdAT, String resOK, String resERR, int espera){
    bool wait = true;
    bool cmd = true;
    int ti = millis();
    
    while(wait && millis() - ti < espera){
        if(cmd){
            Serial1.println(cmdAT);
        }
        cmd = false;
        while(Serial1.available()){
            String res = Serial1.readString();
          
            Serial.println(res);
            if (res.indexOf(resOK) >= 0 ){
                wait = false;
                return true;
            } else if (res.indexOf(resERR) >= 0 ){
                wait = false;
                return false;
            }
        }
    }
    return false;
}

void modem_on(){
    Serial.println("\nprueba de conexion SIM7600 (10seg aprox.)");
    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(300);
    digitalWrite(PWR_PIN, LOW);
    delay(10000);
  
    int i = 10;
    while (i){
        Serial1.println("AT");
        delay(500);
        if (Serial1.available()) {
            String r = Serial1.readString();
            if ( r.indexOf("OK") >= 0 ) {
                reply = true;
                Serial.println("ESTADO OK");
                break;;
            }
        }
        delay(500);
        i--;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(25, OUTPUT);
    digitalWrite(25, LOW);
    delay(2000);
    digitalWrite(25, HIGH);
    Serial1.begin(115200, SERIAL_8N1, 26, 27);

    int retry = 5;
    while (!reply && retry--)
    {
        modem_on();
    }
    delay(2000);
    while(true){
        if(SendCommand("INICIANDO GPS", "AT+CGREG?", "+CGREG: 0,1", "+CGREG: 0,2", 5000))
        {
            break;
        }
        delay(3000);
    }
    Serial.println("registrado en red");
    if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"sslversion\",0,4", "OK", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"authmode\",0,3", "OK", "ERROR", 20000)) //only server verify
        {
            if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"cacert\",0,\"cacert.pem\"", "OK", "ERROR", 20000))
            {
                if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"clientcert\",0,\"cacert.pem\"", "OK", "ERROR", 20000))
                {
                    if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"clientkey\",0,\"privkey.pem\"", "OK", "ERROR", 20000))
                    {
                      Serial.println("configuracion SSL correcta!!");
                    }
                }
            }
        }
    }
    SendCommand("INICIANDO GPS", "AT+CGSOCKCONT=1,\"IP\",\"wap.tmovil.cl\"", "OK", "+NETOPEN: 1", 20000);
    
    SendCommand("INICIANDO GPS", "AT+CSOCKSETPN=1", "OK", "+NETOPEN: 1", 20000);
    SendCommand("INICIANDO GPS", "AT+CIPMODE=0", "OK", "+NETOPEN: 1", 20000);
    if(SendCommand("INICIANDO GPS", "AT+NETOPEN?", "+NETOPEN: 0", "+NETOPEN: 1", 20000))
    {
      //desactivada
      if(SendCommand("INICIANDO GPS", "AT+NETOPEN", "+NETOPEN: 0", "+NETOPEN: 1", 20000))
      {

        //red activada exitosamente
        Serial.println("red lista para iniciar MQTT");
        //red abierta
        SendCommand("INICIANDO GPS", "AT+IPADDR", "OK", "ERROR", 5000);
    
      } else {
        
        //restablece la esp
        digitalWrite(PWR_PIN, HIGH);
        ESP.restart();
      }
    
    }
    SendCommand("INICIANDO GPS", "AT+HTTPINIT", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"CONNECTTO\",120", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"RECVTO\",20", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"ACCEPT\",\"*/*\"", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"UA\",\"PostmanRuntime/7.28.4\"", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"SSLCFG\",0", "OK", "ERROR", 20000);
    
    
    
    
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"USERDATA\",\"Authorization: Bearer " + password +"\"", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"URL\",\"https://cloudiotdevice.googleapis.com/v1/projects/mcd-demos-331211/locations/us-central1/registries/TEST/devices/HNTSTT0002D000002:publishEvent\"", "OK", "ERROR", 20000);
    SendCommand("INICIANDO GPS", "AT+HTTPPARA=\"CONTENT\",\"application/json\"", "OK", "ERROR", 20000);
}
void loop(){
    if(SendCommand("INICIANDO GPS", "AT+HTTPDATA=411,1200", "DOWNLOAD", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", "{\"binary_data\": \"eyJ0eXBlIjoidGVsZW1ldHJ5IiwiZGV2aWNlX2JhdHRlcnkiOjU3LCJkZXZpY2Vfc2lnbmFsIjo3NSwiZGV2aWNlX2h1bWlkaXR5IjoyMywiZGV2aWNlX3RlbXBlcmF0dXJlIjoxMiwibGF0aXR1ZGUiOiAtNzAuNDU3OCwibG9uZ2l0dWRlIjogLTM0LjQ1NjgsInNhdGVsbGl0ZXMiOiA2LCJhbHRpdHVkZSI6IDEyMy40NSwicmZpZF8wIjogIiIsIm9ub2ZmXzEiOiAib2ZmIiwic3BlZWRfMyI6IDEwLjEsInZvbHRhZ2VfNCI6IDE0LjgsImNyZWF0ZWRfYXRfb3JpZ2luIjogIjIwMjEtMTEtMjUgMTk6Mzc6NTkgVVRDIn0=\"}", "OK", "ERROR", 20000))
        {
          if(SendCommand("INICIANDO GPS", "AT+HTTPACTION=1", "200", "ERROR", 20000))
          {
            
            SendCommand("INICIANDO GPS", "AT+HTTPHEAD", "OK", "ERROR", 20000);
            Serial.println("fin");
          }
        }
    }
    delay(3000);
}
