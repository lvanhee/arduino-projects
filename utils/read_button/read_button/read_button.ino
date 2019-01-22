const int buttonPin = 2;
//the other side of the button should be connected to ground

void setup() {
  //start serial connection
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(buttonPin);
  //print out the value of the pushbutton
  Serial.println(sensorVal);
  
  if (sensorVal == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
