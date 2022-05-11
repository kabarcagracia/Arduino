bool save_SD(String file, String data) 
{
  File myFile = SD.open(file, FILE_WRITE);

  if (myFile) {
   
    myFile.print(data);
    myFile.println(",");

    delay(2000);
    myFile.close();
    
    Serial.println("SD save OK");
    return true;
  } else {
    Serial.println("SD save FAILED");
    return false;
  }
}

String format_variable(String var)
{
  if(var.length() == 1 ) 
  {
    return "0" + var;
  } 
  else {
    return var;
  }
}

void check_SD() {

  File root = SD.open("/gps");
  
  int count_files = 0;
  while (true) {

    File entry =  root.openNextFile();
    
    if (! entry) {
      // no more files
      break;
    }
    entry.close();
    count_files ++;
  }
  

  if(count_files > 0)
  {
    Serial.println("Hay " + String(count_files) + " archivos listos para subir...");
    
    File root2 = SD.open("/gps");
    
    while (true)
    {
      File entry2 =  root2.openNextFile();
    
      if (! entry2)
      {
        break;
      }
      String archivo_check = entry2.name();

      entry2.close();
      delay(100);
      
      File readFilefailed = SD.open("/gps/" + archivo_check);
    
       if (readFilefailed) 
       { 
         String datafailed = "";
         while (readFilefailed.available())
         {
           datafailed.concat(readFilefailed.readString());
         }
         readFilefailed.close();
         while(true)
           {
         
           WiFiClient client;
           HTTPClient http3;     
           http3.setTimeout(5000);
           http3.begin(client, SERVER_IP2);
           http3.addHeader("Content-Type", "application/json");
           String data_f = 
           "{\"name_device\" : \"factorb-iot-0000\", "
           "\"data\": [" + datafailed + "{}]}";
           int httpCode3 = http3.POST(data_f);
           Serial.println("posteando archivo");
           
             if (httpCode3 > 0) 
             {
               if (httpCode3 == HTTP_CODE_OK) 
               {
                 const String& payload3 = http3.getString();
                 Serial.println("Archivo recibido exitosamente, borrando archivo temporal");
                 if(SD.remove("gps/" + archivo_check)) 
                 {
                   Serial.println("Archivo Eliminado: " + String(archivo_check));
                 }
                 http3.end();
                 break;
                }
              } 
              else 
              {
                Serial.println("recepcion fallida se reintentara mas tarde");
                http3.end();
                break;
              }




              if (millis() - lastMillis_exit > 6000) 
              {
                lastMillis_exit = millis();
                break;
              
              }
            
            }
         }
      }
  }
  else 
  {
    Serial.println("No hay archivos");
  } 
  
}

String getTimeNtp(){
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
