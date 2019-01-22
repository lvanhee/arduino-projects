#include <SoftwareSerial.h>


SoftwareSerial mySerial(3,2);

const char* ssid     = "SFR-f401";
const char* password = "qhsgzg5kx37c";


void setup() {
  delay(2000);
    Serial.begin(115200);
    mySerial.begin(115200);
    mySerial.println("AT");
    readMySerial();    
    mySerial.println("AT+CWMODE=2");
    readMySerial();
    mySerial.println("AT+CWMODE?");
    readMySerial();
}

void loop() {
  
}

void readMySerial()
{
  int timeout=2000;
  long int time = millis();
  while( (time+timeout) > millis())
   if (mySerial.available()) 
    Serial.write(mySerial.read());
  
}

