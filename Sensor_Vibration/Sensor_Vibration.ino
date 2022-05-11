int inPin = 4;    // pushbutton connected to digital pin 7
bool val = false;
void setup() {
  Serial.begin(9600);
  pinMode(inPin, INPUT);    // sets the digital pin 7 as input
}

void loop() {
  val = digitalRead(inPin);   // read the input pin
  Serial.print("val ->");
  Serial.println(val);
  delay(1000);
}
