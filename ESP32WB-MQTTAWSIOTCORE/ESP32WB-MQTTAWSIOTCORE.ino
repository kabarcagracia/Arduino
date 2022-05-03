#include "FS.h"
#include "SPIFFS.h"
bool reply = false;
#define BAUD 115200
#define TX 27
#define RX 26
#define PWR_PIN 4
#define LED_ESP32 12
#define POWERSIM7600 25
int count = 0;

String proyecto = "mcd-demos-331211";
String ubicacion = "us-central1";
String registro = "KRANOS";
String dispositivo = "HNKRNT0002D000002";
String password = "eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE2Mzg4MDg2MTIsImV4cCI6MTYzODgxNTgxMiwiYXVkIjoibWNkLWRlbW9zLTMzMTIxMSJ9.klE12FtRc8b8RIimaiPLJUgKyEEDesMFh6C9hl0RMQdUsqHherpgmmCa4pRLo2nXe0qWGM-ZuolYNngSnV8CbA";
String username = "unused";
//String broker = "mqtt.2030.ltsapis.goog:8883";
String broker = "a3rnbfkonsql3y-ats.iot.us-east-2.amazonaws.com:8883";
//String topicinit = "/devices/HNKRNT0002D000002/events";
//String msginit = "SIM7600!!!";
String topicinit = "aws/things/SIMCOM7600/";
String msginit = "SIM7600!!!";


bool downloadCert = false;
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

String readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return "empty";
    }

    Serial.println("- read from file:");
    String res = "";
    while(file.available()){
        res.concat(file.readString());
    }
    Serial.println(res);
    file.close();
    return res;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}

bool SendCommand(String cmdAT, String resOK, String resERR, int espera)
{
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
void modem_on()
{
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
  if(!SPIFFS.begin()){
      Serial.println("SPIFFS Mount Failed");
      return;
  }

  //paso 0: descargar certificados
  if(downloadCert){
    /*String caCert = readFile(SPIFFS, "/primbackcert.pem");
    String privateKey = readFile(SPIFFS, "/HNKRNT0002D000002.pem");

    if(SendCommand("AT+CCERTDOWN=\"privkey.pem\"," + String(privateKey.length()) , ">", "ERROR", 2000))
    {
        if(SendCommand(privateKey, "OK", "ERROR", 2000))
        {
          Serial.println("CLAVE PRIVADA OK");
        } else {
          Serial.println("FAILED");
        }
    } else {
      Serial.println("FAILED");
    }

    if(SendCommand("AT+CCERTDOWN=\"cacert.pem\"," + String(caCert.length()) , ">", "ERROR", 2000))
    {
        if(SendCommand(caCert, "OK", "ERROR", 2000))
        {
          Serial.println("CERTIFICADO OK");
        } else {
          Serial.println("FAILED");
        }
    } else {
      Serial.println("FAILED");
    }*/

    String caCert = readFile(SPIFFS, "/AmazonRootCA1.pem");
    String clientCert = readFile(SPIFFS, "/certificate.pem.crt");
    String clientKey = readFile(SPIFFS, "/private.pem.key");

    if(SendCommand("AT+CCERTDOWN=\"cacert.pem\"," + String(caCert.length()) , ">", "ERROR", 2000))
    {
        if(SendCommand(caCert, "OK", "ERROR", 2000))
        {
          Serial.println("CERTIFICADO OK");
        } else {
          Serial.println("FAILED");
        }
    } else {
      Serial.println("FAILED");
    }
    if(SendCommand("AT+CCERTDOWN=\"clientcert.pem\"," + String(clientCert.length()) , ">", "ERROR", 2000))
    {
        if(SendCommand(clientCert, "OK", "ERROR", 2000))
        {
          Serial.println("CERTIFICADO OK");
        } else {
          Serial.println("FAILED");
        }
    } else {
      Serial.println("FAILED");
    }
    if(SendCommand("AT+CCERTDOWN=\"clientkey.pem\"," + String(clientKey.length()) , ">", "ERROR", 2000))
    {
        if(SendCommand(clientKey, "OK", "ERROR", 2000))
        {
          Serial.println("CERTIFICADO OK");
        } else {
          Serial.println("FAILED");
        }
    } else {
      Serial.println("FAILED");
    }
  
  }

  
  //paso 1: asegurar que la red GPRS este disponible
  while(true){
    Serial.println("SEÑAL DE RED: ");
    SendCommand("AT+CSQ", "NO", "+CSQ: 99,99", 2000);
    if(SendCommand("AT+CREG?", "+CREG: 1,1", "+CREG: 0,2", 2000)){
      if(SendCommand("AT+CGREG?", "+CGREG: 0,1", "+CGREG: 0,2", 2000)){
        if(SendCommand("AT+CGSOCKCONT=1,\"IP\",\"wap.tmovil.cl\"", "OK", "+NETOPEN: 1", 20000)){
          if(SendCommand("AT+CGPADDR", "+CGPADDR: 1,", "+CGPADDR: 1,0.0.0.0", 2000)){
            break;
          }
        }
      }
    } else {
      SendCommand("AT+CREG=1", "OK", "ERROR", 2000);
    }
  }

  //paso 2: configurar el contexto PDP
  SendCommand("AT+CGDCONT=1,\"wap.tmovil.cl\",\"0.0.0.0\",0,0", "OK", "ERROR", 2000);
  SendCommand("AT+CSOCKSETPN=1", "OK", "+NETOPEN: 1", 20000);
    //SendCommand("AT+CIPMODE=0", "OK", "+NETOPEN: 1", 20000);
    if(SendCommand("AT+NETOPEN?", "+NETOPEN: 0", "+NETOPEN: 1", 20000))
    {
      //desactivada
      if(SendCommand("AT+NETOPEN", "+NETOPEN: 0", "+NETOPEN: 1", 20000))
      {

        //red activada exitosamente
        Serial.println("red lista para iniciar MQTT");
        //red abierta
        SendCommand("AT+IPADDR", "OK", "ERROR", 5000);
    
      } else {
        
        //restablece la esp
        digitalWrite(PWR_PIN, HIGH);
        ESP.restart();
      }
    
    }
  //paso 3: activar pdp mqtt
  SendCommand("AT+CMQTTSTART", "0", "ERROR", 2000);
  
  //paso 4: crear el cliente
  
  SendCommand("AT+CMQTTACCQ=0,\"SIMCom_client01\",1", "OK", "ERROR", 2000);
  //SendCommand("AT+CMQTTACCQ=0,\"projects/" + proyecto + "/locations/" + ubicacion + "/registries/" + registro + "/devices/" + dispositivo + "\",1", "OK", "ERROR", 2000);
  
  //paso 5: configurar el contexto SSL
  if(SendCommand("AT+CSSLCFG=\"sslversion\",0,4", "OK", "ERROR", 2000)){
    if(SendCommand("AT+CSSLCFG=\"authmode\",0,2", "OK", "ERROR", 2000)){
      if(SendCommand("AT+CSSLCFG=\"ignorelocaltime\",0,1", "OK", "ERROR", 2000)){
        if(SendCommand("AT+CSSLCFG=\"negotiatetime\",0,300", "OK", "ERROR", 20000)){
          /*if(SendCommand("AT+CSSLCFG=\"cacert\",0,\"cacert.pem\"", "OK", "ERROR", 2000)){
            if(SendCommand("AT+CSSLCFG=\"clientcert\",0,\"cacert.pem\"", "OK", "ERROR", 2000)){
              if(SendCommand("AT+CSSLCFG=\"clientkey\",0,\"privkey.pem\"", "OK", "ERROR", 2000)){
              */  
           if(SendCommand("AT+CSSLCFG=\"cacert\",0,\"cacert.pem\"", "OK", "ERROR", 2000)){
            if(SendCommand("AT+CSSLCFG=\"clientcert\",0,\"clientcert.pem\"", "OK", "ERROR", 2000)){
              if(SendCommand("AT+CSSLCFG=\"clientkey\",0,\"clientkey.pem\"", "OK", "ERROR", 2000)){
                Serial.println("configuracion SSL correcta!!");
              }
            }
          }
        }
      }
    }
  }
  SendCommand("AT+CCERTLIST", "OK", "ERROR", 2000);
    //AT+CCERTDOWN
    //AT+CCERTDELE
    //AT+CCERTLIST

  //paso 6: establecer el uso de ssl en mqtt

  SendCommand("AT+CMQTTSSLCFG=0,0", "OK", "ERROR", 2000);

  if(SendCommand("AT+CMQTTWILLTOPIC=0," + String(topicinit.length()), ">", "ERROR", 2000)){
    if(SendCommand(topicinit, "OK", "ERROR", 2000)){
      if(SendCommand("AT+CMQTTWILLMSG=0," + String(msginit.length()) + ",2", ">", "ERROR", 2000)){
        if(SendCommand(msginit, "OK", "ERROR", 2000)){    
        }
      }
    }
  }

  //paso 7: conectar mqtt
  SendCommand("AT+CMQTTCONNECT=0,\"tcp://" + broker + "\",60,1\"unused\",\"unused\"", "+CMQTTCONNECT: 0,0", "ERROR", 2000); 
  //SendCommand("AT+CMQTTCONNECT=0,\"tcp://" + broker + "\",60,0,\"" + username + "\",\"" + password + "\"", "+CMQTTCONNECT: 0,0", "ERROR", 2000);  
}

void loop() {
  SendCommand("AT+CMQTTTOPIC=0,19", ">", "ERROR", 2000);
  SendCommand("aws/things/SIM7600/", "OK", "ERROR", 2000);
  SendCommand("AT+CMQTTPAYLOAD=0,38", ">", "ERROR", 2000);
  SendCommand("{\"message\":\"Hello from SIMCom Module\"}", "OK", "ERROR", 2000);
  SendCommand("AT+CMQTTPUB=0,1,60", "OK", "ERROR", 2000);

}
