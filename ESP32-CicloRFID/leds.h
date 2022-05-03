int LED_AMARILLO = 27;
int LED_ROJO = 26;
int LED_VERDE = 25;
int LED_AZUL = 33;

void initLeds() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_AMARILLO, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);

  digitalWrite(LED_AMARILLO, HIGH);
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  delay(2000);
  
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARILLO, LOW);
  digitalWrite(LED_AZUL, LOW);
}
