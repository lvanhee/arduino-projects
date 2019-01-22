#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(7,8);


int myID = 1; 
int scores[10]={0,0,0,0,0,0,0,0,0,0};


void setup() {
  setupButtons();
  setupRadio();
  setup4x7();
  
  Serial.begin(9600);
  analogReference(INTERNAL);
  
  int bouton = 7; 
  pinMode(7, INPUT_PULLUP);
  delay(1000);
  if(digitalRead(bouton) == HIGH)
  {
      myID = 0;       
  }
  else myID = 1;
  
   
  Serial.print("I am player:");

  Serial.println(myID);
  
}

void loop() {

  loop4x7();
  loopButtons();
  loopRadio();
}
