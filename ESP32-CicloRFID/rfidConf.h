#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 5
#define MOSI_PIN 23
#define MISO_PIN 19
#define SCK_PIN 18
#define RST_PIN 0

byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);

void initRfid(){
    SPI.begin();
    rfid.PCD_Init();
    Serial.print(F("LECTOR: "));
    rfid.PCD_DumpVersionToSerial();
}

void PUBLISH(byte *buffer, byte bufferSize) {
    String dat1 = String(buffer[0], HEX);
    String dat2 = String(buffer[1], HEX);
    String dat3 = String(buffer[2], HEX);
    String dat4 = String(buffer[3], HEX);
    //freerfidonoff O setrfidonoff
    payload =
    "{"
        "\"component_id\": \"rfid_0\","
        "\"currentrfid\": \"" + tag + "\","
        "\"type\": \"device_request\","
        "\"action\": \"setrfidonoff\","
        //"\"relatedcomponent_id\": \"onoff_1\","
        //"\"relatedcomponentvalue\": \"" + onoff + "\","
        "\"newrfid\": \"" + String(buffer[0], HEX) + " " + String(buffer[1], HEX) + " " + String(buffer[2], HEX) + " " + String(buffer[3], HEX) + "\""
    "}";

    Serial.println("ENVIANDO SOLICITUD RFID");
    Serial.println();
    Serial.println(payload);
    publishTelemetry(payload);
    Serial.println();
}

void readRFID(void ) {
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    // Look for new 1 cards
    if ( ! rfid.PICC_IsNewCardPresent())
        return;
  
    // Verify if the NUID has been readed
    if (  !rfid.PICC_ReadCardSerial())
        return;
  
    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
        nuidPICC[i] = rfid.uid.uidByte[i];
    }
    digitalWrite(LED_AMARILLO, HIGH);
    PUBLISH(rfid.uid.uidByte, rfid.uid.size);
    
    // Halt PICC
    rfid.PICC_HaltA();
  
    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
    digitalWrite(LED_AMARILLO, LOW);
}
