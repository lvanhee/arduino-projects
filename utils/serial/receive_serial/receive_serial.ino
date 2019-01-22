//Nano reciever
// read two ints from a Uno
// see Uno_transmitter.ino

#include <SoftwareSerial.h>
int setL, setR;

SoftwareSerial mySerial(4, 5); // RX, TX

void setup()  
{
 
 Serial.begin(4800);
 Serial.flush();
 mySerial.begin(4800);
 mySerial.flush();
 
}

void loop() 
{
 if (mySerial.available()) {   //read the two speeds

   int setL = mySerial.parseInt(); 
   int setR = mySerial.parseInt(); 
   
   //if (mySerial.read() == '\n') {
     Serial.print("setL = ");
     Serial.print(setL);
     Serial.print(", setR = ");
     Serial.println(setR);
   //}  
}
 delay(100);
}
