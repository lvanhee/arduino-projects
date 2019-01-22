#include "vibration.h"
#include "heartbeat_measure.h"
#include "connexionToServer.h"

const int clientID = 2;
const int otherClientID = clientID/2*2+(clientID+1)%2;

int extrapolateDataFromServer(int in)
{
  int minimum = 20; //P=5s
  if(in<60) return minimum;
  if(in>=60&&in <80) return minimum + (minimum* ((double) in-60)/20);// 2*minimum at 80; P=2.5)
  if(in>=80&& in <110) return minimum*2 + (minimum* 2*((double) in-(double)80)/(double)30);
  if(in>=110) return minimum*4;
}


void setup() {
 Serial.begin(115200);
 
 pinMode(LED_BUILTIN,OUTPUT);
 digitalWrite(LED_BUILTIN, LOW);
 Serial.println(otherClientID);
 pinMode(12, OUTPUT);

  for(int i = 0; i<clientID; i++){
    digitalWrite(12, HIGH);
    delay(200);
    digitalWrite(12, LOW);
    delay(200);
  }

  pinMode(A0,INPUT);          // pin that will fade to your heartbeat!
}


 int startTimeOfNextVibration=2000;

 
void loop()
{
  turnOffVibration();
  while(true)
  {
    int myBPM =0;
    bool isBPMCoherentEnoughForBeingShared = false;
    bool resetTime = random(1000)==0;
    if(resetTime)
    {
      Serial.println("RESET!");

    }
          delay(10);
    senseBPM(myBPM, isBPMCoherentEnoughForBeingShared, resetTime);
  int dataFromServer=0;
    
  
   dataFromServer=updateDataWithServer(
    clientID,
    otherClientID,
    myBPM,
    startTimeOfNextVibration);

  if(dataFromServer>0 &&
 !isOutOfSync)
    startTimeOfNextVibration= updateVibration(extrapolateDataFromServer(dataFromServer));
  else
  {
    //Serial.println("No server");
    turnOffVibration();
  }
  }
}



