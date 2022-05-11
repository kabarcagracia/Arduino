bool SendCIPOPEN(String cmdAT, int espera)
{
    bool wait = true;
    bool cmd = true;
    int ti = millis();
    
    while(wait && millis() - ti < espera)
    {
        if(cmd)
        {
            SIM7600.println(cmdAT);
        }
        cmd = false;
        
        while(SIM7600.available())
        {
            String res = SIM7600.readString();
            
            MONITOR.println(res);
            if (res.indexOf("1,0") >= 0 )
            {
                MONITOR.println("CONEXION ESTABLECIDA");
                wait = false;
               
                return true;   
            } 
            else if (res.indexOf("1,4") >= 0 )
            {
                MONITOR.println("OPERACION NO SOPORTADA");
                wait = false;
                
                return true;                
            }
            else if (res.indexOf("1,10") >= 0 )
            {
                MONITOR.println("TIMEOUT, SERVIDOR NO RESPONDE");
                wait = false;
              
                return false;
            }
            else if (res.indexOf("1,2") >= 0 )
            {
                MONITOR.println("CONEXION CERRADA");
                wait = false;
               
                return false;
            }
        }
    }
    return false;
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
            SIM7600.println(cmdAT);
        }
        cmd = false;
        
        while(SIM7600.available())
        {
            String res = SIM7600.readString();
            
            MONITOR.println(res);
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

String gps_success(){
    bool status_gps = false;
    bool gps = false;
    bool whilegps = true;
    bool cmdgps = true;
    
    while(whilegps) 
    {
        if(cmdgps)
        {
            SIM7600.write("AT+CGPSINFO\r");
        }
        cmdgps = false;
        
        while(SIM7600.available()) 
        {
            String res = SIM7600.readString();
            MONITOR.println(res);
            if ( res.indexOf("OK") >= 0 ) 
            {
                whilegps = false;
                String e = s.separa(res, ':', 1);

                //LATITUD
                String lat_sxg = s.separa(e, ',', 0);
                String lat_sxg_h = lat_sxg.substring(0,3);
                String lat_sxg_m = lat_sxg.substring(3);
                String lat_o = s.separa(e, ',', 1);
                float latitud = lat_sxg_h.toFloat() + (lat_sxg_m.toFloat()/60);
                if(lat_o == "S")
                {
                    latitud = latitud * (-1);
                }
                else if(lat_o == "N")
                {
                    latitud = latitud * (1);
                }

                //LONGITUD
                String lng_sxg = s.separa(e, ',', 2);
                String lng_sxg_h = lng_sxg.substring(0,3);
                String lng_sxg_m = lng_sxg.substring(3);   
                String lng_o = s.separa(e, ',', 3);
                float longitud = lng_sxg_h.toFloat() + (lng_sxg_m.toFloat()/60);
                if(lng_o == "W")
                {
                    longitud = longitud * (-1);
                }
                else if(lng_o == "E")
                {
                    longitud = longitud * (1);
                }
        
                //FECHA
                String fecha = s.separa(e, ',', 4);
                String dia = fecha.substring(0,2);
                String mes = fecha.substring(2,4);
                String ano = fecha.substring(4,6);
                ano = "20" + ano;
                fecha = ano + "-" + mes + "-" + dia;
          
                //HORA
                String tiempo = s.separa(e, ',', 5);
                String hora = tiempo.substring(0,2);
                String minutos = tiempo.substring(2,4);
                String segundos = tiempo.substring(4,6);
                tiempo = hora + ":" + minutos + ":" + segundos;

                //DATETIME
                String datetime = fecha + "T" + tiempo + "Z"; 
                archivo = ano+mes+dia+"T"+hora+minutos+segundos+".txt";
                String velocidad = s.separa(e, ',', 7);
                
                //VELOCIDAD
                float vel = velocidad.toFloat() * 1.852;
                
                String formatData = 
                "{"
                    "\"name_device\": \"factorb-iot-0018\","
                    "\"latitude\":" + String(latitud, 7) + ","
                    "\"longitude\":" + String(longitud, 7) + ","
                    "\"speed\":" + String(vel) + ","
                    "\"time\": \"" + datetime + "\""
                "}";
                if(latitud == 0.000000 || longitud == 0.00000)
                {
                    status_gps = false;
                } else
                {
                    status_gps = true; 
                }

                if(status_gps)
                {
                    return formatData;    
                }
                else
                {
                    return "";
                }
            }
            else if ( res.indexOf("ERROR") >= 0 ) 
            {
                whilegps = false;
                return "";
            }
        }
    }
}


void save_data(String file, String data_f)
{
  File myFile = SD.open(file, FILE_WRITE);

  if (myFile) 
  {
    myFile.println(data_f);
    delay(3000);
    myFile.close();
    
    MONITOR.println("SD save OK");
  } else {
    MONITOR.println("SD save FAILED");
  }
}

bool tcp_start(){
  delay(1000);
      if(SendCommand("", "AT+CIPTIMEOUT=10000,10000,5000", "OK", "ERROR", 3000)) //AQUI ES AL REVEZ
      {
          MONITOR.println("AT+CIPTIMEOUT --> OK");
          if(SendCommand("", "AT+NETOPEN", "OK", "ERROR", 3000))
          {
            MONITOR.println("RED ABIERTA OK");
               if(SendCIPOPEN("AT+CIPOPEN=1,\"TCP\",\"34.230.28.13\",51021", 10000))
               {
                   return true;
               }
          }
      }
      return false;
}

bool tcp_restart(){
  MONITOR.println("\n\n\n\n\nrestablecer esta mierda");
  if(SendCIPOPEN("AT+CIPOPEN=1,\"TCP\",\"34.230.28.13\",51021", 10000))
  {
    MONITOR.println("RESTABLECIDA");
    return true;
  }

  return false;
}



bool tcp_restart2(){
  
  
}

bool SendDATA(String cmdAT, int espera)
{
    bool wait = true;
    bool cmd = true;
    int ti = millis();
  
    //MONITOR.println(msg);
    while(wait && millis() - ti < espera)
    {
        if(cmd)
        {
            SIM7600.println(cmdAT);
        }
        cmd = false;
        
        while(SIM7600.available())
        {
            String res = SIM7600.readString();
            
            MONITOR.println(res);
            if (res.indexOf("+IPD2") >= 0 )
            {
                wait = false;
                return true;
            } 
            else if (res.indexOf("+CIPERROR: 2") >= 0 )
            {
                MONITOR.println("red no abierta, es necesario abrir");
                wait = false;
                return false;
            }
            else if (res.indexOf("+CIPERROR: 4") >= 0 )
            {
                MONITOR.println("operacion no soportada");
                wait = false;
                return false;
            }
        }
    }
    return false;
}
 
void publish_data(){
  bool publicado = false;
  String datadevice = gps_success();
  if(datadevice != "")
  {
    if(SendCommand("", "AT+CIPSEND=1," + String(datadevice.length()), ">", "ERROR", 2000))
    {
      if(SendDATA(datadevice, 5000))
      {
        publicado = true;
        count = 0;
        conexion_establecida  = true;
        MONITOR.println("publicado");
      }
    }
    
    if(!publicado)
    {
      MONITOR.println(archivo);
      if(datadevice.length() >= 0){
        save_data("/data/" + archivo, datadevice);
        conexion_establecida = false;
        count++;
        //arreglar reinicio
        MONITOR.println(count);
        if(count > 5)
        {
          tcp_restart();
        }
     } else {
      MONITOR.println("trama sin datos"); 
     }
      
      

    }
  } 
}

bool publish_data2(String file)
{
  if(SendCommand("", "AT+CIPSEND=1," + String(file.length()), ">", "ERROR", 2000))
  {
    if(SendCommand("", file, "+IPD2", "+CIPERROR", 5000))
    {
      return true;
    }
  }
  return false;  
}

void modem_on()
{
  MONITOR.println("\nprueba de conexion SIM7600 (10seg aprox.)");
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, HIGH);
  delay(300);
  digitalWrite(PWR_PIN, LOW);
  delay(10000);

  int i = 10;
  while (i) 
  {
    SIM7600.println("AT");
    delay(500);
    if (SIM7600.available()) 
    {
      String r = SIM7600.readString();
      if ( r.indexOf("OK") >= 0 ) 
      {
        reply = true;
        MONITOR.println("ESTADO OK");
        break;;
      }
    }
    delay(500);
    i--;
  }
}

void check_SD(){
  File root = SD.open("/data");
  
  int count_files = 0;
  while (true)
  {
    File entry =  root.openNextFile();
    
    if (! entry)
    {
      // no more files
      break;
    }
    entry.close();
    count_files ++;
  }
  MONITOR.println("SD revisada: " + String(count_files) + "archivos");
  int contador_archivos_env = 0;
  if(count_files > 0)  
  {
    File root2 = SD.open("/data");
    while (true)
    {
      if(contador_archivos_env > 1) {    
        break;
      }
      
      File entry2 =  root2.openNextFile();
    
      if (! entry2)
      {
        break;
      }
      String archivo_check = entry2.name();
      
      entry2.close();
      delay(100);
      
      File readFilefailed = SD.open(archivo_check);
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
          if(datafailed.length() <= 0){
              if(SD.remove(archivo_check)) 
              {
                  MONITOR.println("Archivo Eliminado[0 bytes]: " + String(archivo_check));
              } else {
                  MONITOR.println("no Eliminado");
              }
          }  
          if(publish_data2(datafailed))
          {
            MONITOR.println("Archivo recibido exitosamente, borrando archivo temporal");
            MONITOR.println(archivo_check);
            contador_archivos_env++;
            if(SD.remove(archivo_check)) 
            {
              MONITOR.println("Archivo Eliminado: " + String(archivo_check));
              break;;
            } else {
              MONITOR.println("no Eliminado");
              break;;
            }
          }
          else {
            MONITOR.println(archivo_check);
            MONITOR.println("recepcion fallida se reintentara mas tarde");
            contador_archivos_env++;
            break;;
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
