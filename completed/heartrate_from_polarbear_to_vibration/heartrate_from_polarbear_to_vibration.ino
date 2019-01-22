#include "BLEDevice.h"
#include "receiveHB.h"
#include "display.h"
//#include <WiFi.h>



void setup() {
  Serial.begin(115200);
    setupDisplay();


 // WiFi.disconnect(true); delay(1); // disable WIFI altogether
  //WiFi.mode(WIFI_OFF); delay(1);
  setupReceiveHeartRate();
  
  pinMode(2, OUTPUT);
} 

long timeSinceStart = millis();
void loop() {
  loopReceiveHeartRate();

  if(getHR()<=0 && millis() > timeSinceStart +30000){Serial.println("Lost signal, rebooting"); ESP.restart();}

  loopDisplay(getHR());

   

   
} // End of loop
