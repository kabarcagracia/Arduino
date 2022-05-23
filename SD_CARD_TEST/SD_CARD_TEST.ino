#include <SD.h>
//biblioteca responsável pela comunicação com o sensor DHT22
#include "DHTesp.h"

#define DHTpin 4    //D5 of NodeMCU is GPIO14

DHTesp dht;
//pino ligado ao CS do módulo SD Card
#define CS_PIN 15

void setup()
{
  Serial.begin(9600);
  Serial.print("Inicializando o cartão SD...");
 
  //inicializa o objeto para comunicarmos com o sensor DHT
  dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
   
  // verifica se o cartão SD está presente e se pode ser inicializado
  if (!SD.begin(CS_PIN)) {
    Serial.println("Falha, verifique se o cartão está presente.");
    //programa encerrrado
    return;
  }
   
  //se chegou aqui é porque o cartão foi inicializado corretamente  
  Serial.println("Cartão inicializado.");
 
}

void loop() {
  //faz a leitura da umidade

  delay(dht.getMinimumSamplingPeriod());

  float umidade = dht.getHumidity();
  float temperatura = dht.getTemperature();
  
  Serial.print("Umidade: ");
  Serial.println(umidade);
  //faz a leitura da temperatura
  
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
 
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // se o arquivo foi aberto corretamente, escreve os dados nele
  Serial.print("Status SD Card:  ");Serial.println(dataFile);
  if (dataFile) {
    Serial.println("O arquivo foi aberto com sucesso.");
      //formatação no arquivo: linha a linha >> UMIDADE | TEMPERATURA
      dataFile.print(umidade);
      dataFile.print(" | ");
      dataFile.println(temperatura);
 
      //fecha o arquivo após usá-lo
      dataFile.close();
  }
  // se o arquivo não pôde ser aberto os dados não serão gravados.
  else {
    Serial.println("Falha ao abrir o arquivo LOG.txt");
  }
 
  //intervalo de espera para uma nova leitura dos dados.
  delay(2000);
 
}
