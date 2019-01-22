//Uno transmitter
// Send two ints to a Nano (wheel speed setpoints)
// Connect pin5 Uno to pin 4 Nano
// Connect pin4 Uno to pin 5 Nano
// Receiver is Nano_reciever.ino

#include <SoftwareSerial.h>
int speedL, speedR;

SoftwareSerial mySerial(4, 5); // RX, TX

void setup()  
{
 
 Serial.begin(4800);
 Serial.flush();
 mySerial.begin(4800);
 mySerial.flush();

 speedL = 123;          // for testing
 speedR = -456;
}

void loop() 
{
 mySerial.print(speedL);            //send the two speeds
 mySerial.print(",");
 mySerial.print(speedR);
 mySerial.print('\n');
 
 Serial.print(speedL);            //just for debugging
 Serial.print(",");
 Serial.print(speedR);
 Serial.print('\n');
 
 delay(100);
}

