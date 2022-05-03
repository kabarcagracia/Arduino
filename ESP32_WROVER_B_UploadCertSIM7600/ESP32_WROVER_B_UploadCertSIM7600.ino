#include "FS.h"
#include "SPIFFS.h"
bool reply = false;
#define BAUD 115200
#define TX 27
#define RX 26
#define PWR_PIN 4
#define LED_ESP32 12
#define POWERSIM7600 25

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

bool SendCommand(String msg, String cmdAT, String resOK, String resERR, int espera)
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


void setup(){
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
  listDir(SPIFFS, "/", 0);
  String CACert = readFile(SPIFFS, "/primback.pem");
  String clientCert = readFile(SPIFFS, "/pubKey.pem");
  String clientKey = readFile(SPIFFS, "/privKey.pem");

  delay(2000);
    if(SendCommand("INICIANDO GPS", "AT", "OK", "ERROR", 20000))
    {
        Serial.println("OK PERRIX");
    } else {
      Serial.println("cago");
    }

    if(SendCommand("INICIANDO GPS", "AT+CCERTDOWN=\"privkey.pem\"," + String(clientKey.length()) , ">", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", clientKey, "OK", "ERROR", 20000))
        {
          Serial.println("METALE TALENTO PARIENTE");
        } else {
          Serial.println("cago");
        }
    } else {
      Serial.println("cago");
    }

    if(SendCommand("INICIANDO GPS", "AT+CCERTDOWN=\"cacert.pem\"," + String(CACert.length()) , ">", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", CACert, "OK", "ERROR", 20000))
        {
          Serial.println("METALE TALENTO PARIENTE");
        } else {
          Serial.println("cago");
        }
    } else {
      Serial.println("cago");
    }

    if(SendCommand("INICIANDO GPS", "AT+CCERTDOWN=\"clientcert.pem\"," + String(clientCert.length()) , ">", "ERROR", 20000))
    {
        if(SendCommand("INICIANDO GPS", clientCert, "OK", "ERROR", 20000))
        {
          Serial.println("METALE TALENTO PARIENTE");
        } else {
          Serial.println("cago");
        }
    } else {
      Serial.println("cago");
    }


    Serial.println("ARCHIVOS CARGADOS!!!");

  

  



  
}

void loop(){}
