bool SendCommand(String msg, String cmdAT, String resOK, String resERR)
{
  bool wait = true;
  bool cmd = true;
  
  //MONITOR.println(msg);
  while(wait)
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
        
        String datetime = fecha + "T" + tiempo + "Z"; 
       
          
        String velocidad = s.separa(e, ',', 9);

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
      else if ( res.indexOf("ERROR") >= 0 ) {
        whilegps = false;
        return "";
      }
    }
  }
}
void publicar_data(String data_f){
  int largo  = data_f.length();
  
//  AT+CMQTTTOPIC=0,20

///devices/test/events

//AT+CMQTTPAYLOAD=0,20

//{"msg":123456789012}

//AT+CMQTTPUB=0,1,60
  bool pub = false;
  if(SendCommand("TOPIC", "AT+CMQTTTOPIC=0,8", ">", "ERROR"))
  {
    if(SendCommand("NAME TOPIC", "/devices", "OK", "ERROR"))
    {
      if(SendCommand("PAYLOAD LENGTH", "AT+CMQTTPAYLOAD=0,"+ String(largo), ">", "ERROR"))
      {
        if(SendCommand("PAYLOAD", data_f, "OK", "ERROR"))
        {
          if(SendCommand("PUBLICAR", "AT+CMQTTPUB=0,1,60", "0,0", "ERROR"))
          {
            /*if(SendCommand("suscribirse", "AT+CMQTTSUBTOPIC=0,6,1", ">", "ERROR"))
            {
              if(SendCommand("topic sub", "/input", "OK", "ERROR"))
              {
                if(SendCommand("subscrito", "AT+CMQTTSUB=0", "OK", "ERROR"))
                {
            
                  MONITOR.println("publicado");
                  pub = true;
                }
              }
            }*/
            MONITOR.println("publicado");
                  pub = true;
          }
        }
      } 
    }
  }

  if(pub) 
  {
    //PERFECTO  
  } 
  else {

    //GUARDAR SD
  MONITOR.println("NONONON");
  }
}
bool mqtt_start(){
  delay(10000);
  SendCommand("ABRIR RED", "AT+NETOPEN?", "OK", "0");
  if(SendCommand("INICIAR MQTT", "AT+CMQTTSTART", "OK", "ERROR"))
  {
    if(SendCommand("CLIENTE MQTT", "AT+CMQTTACCQ=0,\"factorb\",0", "OK", "ERROR"))
    {
      if(SendCommand("TOPIC CONEXION", "AT+CMQTTWILLTOPIC=0,8", ">", "ERROR"))
      {
        if(SendCommand("TOPIC", "/devices", "OK", "ERROR"))
        {
          if(SendCommand("MSG LENGTH", "AT+CMQTTWILLMSG=0,9,1", ">", "ERROR"))
          {
            if(SendCommand("MENSAJE", "connected", "OK", "ERROR"))
            {
              if(SendCommand("CONEXION MQTT", "AT+CMQTTCONNECT=0,\"tcp://34.230.28.13:1883\",60,1,\"SIM7600SA\",\"SIM7600SAPWD\"", "0,0", "ERROR"))
              {
                /*if(SendCommand("TOPIC", "AT+CMQTTTOPIC=0,8", ">", "ERROR"))
                {
                  if(SendCommand("NAME TOPIC", "/devices", "OK", "ERROR"))
                  {
                    if(SendCommand("PAYLOAD LENGTH", "AT+CMQTTPAYLOAD=0,20", ">", "ERROR"))
                    {
                      if(SendCommand("PAYLOAD", "{\"msg\":123456789098}", "OK", "ERROR"))
                      {
                        if(SendCommand("PUBLICAR", "AT+CMQTTPUB=0,1,60", "0,0", "ERROR"))
                        {*/
                          
                                MONITOR.println("publicado");
                                return true;
                        }/*
                      }
                    } 
                  }
                }
              }*/
            }
          }  
        }
      }
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
