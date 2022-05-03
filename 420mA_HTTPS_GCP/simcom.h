bool reply = false;
#define LED_ESP32 12

void modem_on(){
    Serial.print("Inicializando SIM7600SA");
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    delay(300);
    digitalWrite(4, LOW);
    delay(10000);
  
    int i = 10;
    while (i){
        Serial.print(".");
        Serial1.println("AT");
        delay(500);
        if (Serial1.available()) {
            String r = Serial1.readString();
            if ( r.indexOf("OK") >= 0 ) {
                reply = true;
                Serial.println("");
                Serial.println("<<<  SIMCOM LISTA  >>>");
                break;;
            }
        }
        delay(500);
        i--;
    }
}

void initModem(){
    pinMode(25, OUTPUT);
    digitalWrite(25, LOW);
    delay(2000);
    digitalWrite(25, HIGH);
    Serial1.begin(115200, SERIAL_8N1, 26, 27);

    int retry = 5;
    while (!reply && retry--){
        modem_on();
    }
    delay(2000);  
}

bool SendCommand(String cmdAT, String resOK, String resERR, int espera){
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
String SendCommand2(String cmdAT, String resOK, String resERR, int espera){
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
               
            } else if (res.indexOf(resERR) >= 0 ){
                wait = false;
               
            }

            return res;
        }
    }
    return "";
}
