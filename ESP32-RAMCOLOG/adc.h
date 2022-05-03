const int pin_adc_24v = 36;
const int pin_adc_3v7 = 39;

int adc_24v = 0;
int adc_3v7 = 0;

float voltage_adc_24v = 0.0;
float voltage_adc_3v7 = 0.0;

float voltage_24v = 0.0;
float voltage_3v7 = 0.0;

float porcent = 0.0;
float mapa(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

String getBatExt(){
    adc_24v = analogRead(pin_adc_24v);
  //  Serial.print("ADC 24V: ");
  //  Serial.print(adc_24v);
    voltage_adc_24v = (adc_24v * 3.3)/4095;
  //  Serial.print("\tADC 24V [V]: ");
  //  Serial.print(voltage_adc_24v);
    voltage_24v = voltage_adc_24v / (1.2 / (1.2 + 10.0));
  //  Serial.print("\t 24V [V]: ");
  //  Serial.println(voltage_24v);
    return String(voltage_24v, 2);
}
String getBatInt(){
    adc_3v7 = analogRead(pin_adc_3v7);
  //  Serial.print("ADC 3V7: ");
  //  Serial.print(adc_3v7);
    voltage_adc_3v7 = (adc_3v7 * 3.3)/(4095); 
  //  Serial.print("\tADC 3V7 [V]: ");
  //  Serial.print(voltage_adc_3v7);
    voltage_3v7 = voltage_adc_3v7 / ( 1.7/( 1.0 + 1.7));
  //  Serial.print("\t 3V7 [V]: ");
  //  Serial.print(voltage_3v7);
    porcent = mapa(voltage_3v7, 0.0, 4.2, 0, 100);
  //  Serial.print("\t 3V7 [%]: ");
  //  Serial.println(int(porcent));
    return String(int(porcent));
}
