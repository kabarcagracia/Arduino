bool reply = false;
#define BAUD 115200
#define TX 27
#define RX 26
#define PWR_PIN 4
#define LED_ESP32 12
#define POWERSIM7600 25
int count = 0;

String registry = "TEST";
String projects = "mcd-demos-331211";
String location = "us-central1";
String device = "HNTSTT0002D000002";

String topicinit = "devices/HNTSTT0002D000002/events";
String msginit = "SIM7600 Connected!";

String broker = "mqtt.2030.ltsapis.goog";
String port = "8883";

String user = "unused";
String password = "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE2Mzc5Mzg2NzUsImV4cCI6MTYzNzk1MzA3NSwiYXVkIjoibWNkLWRlbW9zLTMzMTIxMSJ9.UODgn1w_8SC86kZh9h2RR5NkgW7oelGjSMx_qCPukPQ2B_cVtkeoyHVoGlvcGGjijWWP3LBI0A4vTzFRm3w50A";

bool SendCommand(String msg, String cmdAT, String resOK, String resERR, int espera){
    bool wait = true;
    bool cmd = true;
    int ti = millis();
  
    //MONITOR.println(msg);
    while(wait && millis() - ti < espera)
    {
        if(cmd)
        {
            //Serial.println(cmdAT);
            Serial1.println(cmdAT);
        }
        cmd = false;
        
        while(Serial1.available())
        {
            String res = Serial1.readString();
            
            Serial.println(res);
            if (res.indexOf(resOK) >= 0 )
            {
                wait = false;
                return true;
            } 
            else if (res.indexOf(resERR) >= 0 )
            {
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
  while (i) 
  {
    Serial1.println("AT");
    delay(500);
    if (Serial1.available()) 
    {
      String r = Serial1.readString();
      if ( r.indexOf("OK") >= 0 ) 
      {
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

    SendCommand("INICIANDO GPS", "AT+CCERTLIST", "OK", "ERROR", 20000);
    
    if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"sslversion\",0,4", "OK", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"authmode\",0,1", "OK", "ERROR", 20000)) //only server verify
        {
            if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"cacert\",0,\"primbackcert.pem\"", "OK", "ERROR", 20000))
            {
                if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"clientcert\",0,\"primbackcert.pem\"", "OK", "ERROR", 20000))
                {
                    if(SendCommand("INICIANDO GPS", "AT+CSSLCFG=\"clientkey\",0,\"privkey.pem\"", "OK", "ERROR", 20000))
                    {
                      Serial.println("configuracion SSL correcta!!");
                      
                    }
                }
            }
        }
    }
    //AT+CCHSET=1
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

    /*if(SendCommand("INICIANDO GPS", "AT+CMQTTSTART", "0", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", "AT+CMQTTACCQ=0,\"projects/" + projects + "/locations/" + location + "/registries/" + registry + "/devices/" + device + "\",1", "OK", "ERROR", 20000))
        {
            if(SendCommand("INICIANDO GPS", "AT+CMQTTCFG=\"checkUTF8\",0,0", "OK", "ERROR", 20000))
            {
                if(SendCommand("INICIANDO GPS", "AT+CMQTTSSLCFG=0,0", "OK", "ERROR", 20000))
                {
                   
                    if(SendCommand("INICIANDO GPS", "AT+CMQTTWILLTOPIC=0," + String(topicinit.length()), ">", "ERROR", 20000))
                    {
                       
                        if(SendCommand("INICIANDO GPS", topicinit, "OK", "ERROR", 20000))
                        {
                            if(SendCommand("INICIANDO GPS", "AT+CMQTTWILLMSG=0," + String(msginit.length()) + ",1", ">", "ERROR", 20000))
                            {
                                if(SendCommand("INICIANDO GPS", msginit, "OK", "ERROR", 20000))
                                {
                                    if(SendCommand("INICIANDO GPS", "AT+CMQTTCONNECT=0,\"tcp://" + broker + ":" + port + "\",60,0,\"" + user + "\",\"" + password + "\"", "+CMQTTCONNECT: 0,0", "ERROR", 20000))
                                    {
                                      
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }*/

    if(SendCommand("INICIANDO GPS", "AT+CMQTTSTART", "0", "ERROR", 20000))
    {
        //if(SendCommand("INICIANDO GPS", "AT+CMQTTACCQ=0,\"projects/mcd-demos-331211/locations/us-central1/registries/TEST/devices/HNTSTT0002D000002\",1", "OK", "ERROR", 20000))
        if(SendCommand("INICIANDO GPS", "AT+CMQTTACCQ=0,\"mqttx_744cc48e\"", "OK", "ERROR", 20000))
        {
            //if(SendCommand("INICIANDO GPS", "AT+CMQTTCFG=\"checkUTF8\",0,0", "OK", "ERROR", 20000))
            //{
                
                    //if(SendCommand("INICIANDO GPS", "AT+CMQTTWILLTOPIC=0,32", ">", "ERROR", 20000))
                    if(SendCommand("INICIANDO GPS", "AT+CMQTTWILLTOPIC=0," + String(topicinit.length()), ">", "ERROR", 20000))
                    {
                        //if(SendCommand("INICIANDO GPS", "devices/HNTSTT0002D000002/events", "OK", "ERROR", 20000))
                        if(SendCommand("INICIANDO GPS", topicinit, "OK", "ERROR", 20000))
                        {
                            if(SendCommand("INICIANDO GPS", "AT+CMQTTWILLMSG=0," + String(msginit.length()) + ",2", ">", "ERROR", 20000))
                            {
                                if(SendCommand("INICIANDO GPS", msginit, "OK", "ERROR", 20000))
                                {
                                    if(SendCommand("INICIANDO GPS", "AT+CMQTTCONNECT=0,\"tcp://hdb9bd2c.us-east-1.emqx.cloud:15055\",60,1,\"unused\",\"unused\"", "+CMQTTCONNECT: 0,0", "ERROR", 20000))
                                    {
                                      SendCommand("INICIANDO GPS", "AT+CMQTTTOPIC=0,10", ">", "ERROR", 20000);
                                      SendCommand("INICIANDO GPS", "simcomtest", "OK", "0", 20000);
                                      SendCommand("INICIANDO GPS", "AT+CMQTTPAYLOAD=0,9", ">", "ERROR", 20000);
                                      SendCommand("INICIANDO GPS", "mqtt_test", "OK", "0", 20000);
                                      SendCommand("INICIANDO GPS", "AT+CMQTTPUB=0,1,60", ">", "ERROR", 20000);


                                   

                                      
                                    }
                                }
                            }
                        }
                    }
                
            //}
        }
    }
    

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    Serial.write(Serial1.read());
  }
 
  if(Serial.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  {
     Serial1.write(Serial.read());
  }
}
