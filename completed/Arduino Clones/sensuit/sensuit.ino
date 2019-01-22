/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/

int LEDtoLight=0;

// the setup function runs once when you press reset or power the board
void setup() {
  //Serial.begin(115200);
    randomSeed(analogRead(A0));

 pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

    digitalWrite(18, HIGH);  

  int testDelay = 1000;
   for(int i = 0 ; i <= 19 ; i ++)
  {
    if(i!=4 && i!=18)
    {
    digitalWrite(i, HIGH);  
  delay(testDelay);               
  digitalWrite(i, LOW);   
    }
  }

  
  
 /* digitalWrite(A0, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(testDelay);                       // wait for a second
  digitalWrite(A0, LOW);    // turn the LED off by making the voltage LOW
  
  digitalWrite(A1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(testDelay);                       // wait for a second
  digitalWrite(A1, LOW);    // turn the LED off by making the voltage LOW
  
  digitalWrite(A2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(testDelay);                       // wait for a second
  digitalWrite(A2, LOW);    // turn the LED off by making the voltage LOW
  
  digitalWrite(A3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(testDelay);                       // wait for a second
  digitalWrite(A3, LOW);    // turn the LED off by making the voltage LOW
  
  digitalWrite(A4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(testDelay);                       // wait for a second
  digitalWrite(A4, LOW);    // turn the LED off by making the voltage LOW
  
  digitalWrite(A5, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(testDelay);                       // wait for a second
  digitalWrite(A5, LOW);    // turn the LED off by making the voltage LOW
  */
  /*int LEDtoLight=random(19);

    Serial.println(LEDtoLight);*/

}


int delayDuration = 15;
float factor = 0;
long lastTick = millis();
long lastLEDUpdate = millis();
void loop() {

  if(lastTick + 1000 <millis())
  {
    factor +=0.01;
    if(factor > 1) factor = 1;
     lastTick = millis();
   //  Serial.println("tick!");
  }

  //Serial.println(lastLEDUpdate);
  //Serial.println(millis()); 

  if(lastLEDUpdate + 180000 <millis())
  {
    int tmp = LEDtoLight;
    while(LEDtoLight == tmp || LEDtoLight==4 || LEDtoLight == 18)
      LEDtoLight = random(19);
   lastLEDUpdate = millis();

   Serial.println(LEDtoLight);
   factor = 0;
  }

 digitalWrite(LEDtoLight, HIGH);
 delay(delayDuration*factor);
 digitalWrite(LEDtoLight, LOW);
 delay(delayDuration*(1-factor));

}
