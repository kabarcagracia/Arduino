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
bool statusCommand1 = true;
bool statusCommand2 = false;
void setup(){
    Serial.begin(115200); 
  
    pinMode(25, OUTPUT);
    digitalWrite(25, LOW);
    delay(2000);
    digitalWrite(25, HIGH);
    
    Serial1.begin(115200, SERIAL_8N1, 26, 27);
    xTaskCreatePinnedToCore(TaskBlink,"Taskname",1024,NULL,1,NULL,1);
}
void loop(){
    delay(5000);
    if(statusCommand1)
        Serial1.println("AT+CSSLCFG=\"sslversion\",0,3");
    if(statusCommand2)
        Serial1.println("AT+CSSLCFG=\"authmode\",0,0");
}

void TaskBlink(void *pvParameters) {
    bool imprimir = false;
    String response = "";
    for (;;) {
        if(Serial1.available()){
            response = Serial1.readString();
            imprimir = true;
        }
        if(imprimir){
            Serial.println("===");
            Serial.println(response);
            Serial.println("===");
            if(response.indexOf("OK") >= 0){
                if(response.indexOf("AT+CSSLCFG=\"sslversion\",0,3") >= 0) {
                    statusCommand1 = false;
                    statusCommand2 = true;
                    Serial.println("encontrado 1");
                }
                else if(response.indexOf("AT+CSSLCFG=\"authmode\",0,0") >= 0) {
                    statusCommand2 = false;
                    Serial.println("encontrado 2");
                }
            }
            else if(response.indexOf("ERROR") >= 0) {
              Serial.println("comando erroneo");
            }
        }
        response = "";
        imprimir = false;
        delay(10);
    }
}
