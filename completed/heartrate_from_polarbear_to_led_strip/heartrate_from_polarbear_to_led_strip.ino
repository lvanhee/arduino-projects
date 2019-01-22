#include "BLEDevice.h"
#include "receiveHB.h"
#include "display.h"
#include "accelerometer.h"
#include <WiFi.h>



void setup() {
  Serial.begin(115200);

 /* WiFi.disconnect(true); delay(1); // disable WIFI altogether
  WiFi.mode(WIFI_OFF); delay(1);*/
  setupReceiveHeartRate();
  setupDisplay();
  setupAccelero();
  
  pinMode(2, OUTPUT);
} 

void loop() {
  loopReceiveHeartRate();

  int x; 
  int y;
  int z;
  loopAccelero(x, y, z);

 // Serial.println(getHR());

  loopDisplay(getHR(),x,y,z);
   

   
} // End of loop
