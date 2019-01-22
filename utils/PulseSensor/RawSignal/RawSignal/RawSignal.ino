void setup() {
 pinMode(13,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(A0,INPUT);          // pin that will fade to your heartbeat!
  Serial.begin(115200);             // we agree to talk fast!
  
}

void loop() {
    Serial.println(analogRead(A0));
    delay(10);
}

