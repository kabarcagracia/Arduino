const int rele = D4;
void setup() 
{
  pinMode(rele, OUTPUT);
  Serial.begin(9600);
}
 
void loop() 
{
  float sensorVoltage; 
  float sensorValue;
  float porcent;
 
  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/1024*5.0;
  porcent = sensorValue/100;
  //Serial.print("sensor voltage = ");
  //Serial.print(sensorVoltage);
  //Serial.println(" V");
  Serial.println(porcent);
  //Serial.println(" %");
  
  delay(1000);
}
