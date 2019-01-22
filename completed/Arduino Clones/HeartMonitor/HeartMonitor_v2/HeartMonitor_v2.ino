#include "vibration.h"
#include "heartbeat_measure.h"
#include "connexionToServer.h"

const int clientID =1;
const int otherClientID = clientID/2*2+(clientID+1)%2;

int extrapolateDataFromServer(int in)
{
  if(in <= 0 )return -1;
  if(in < 60) return 5;
  if(in > 120) return 80;
  return (in - 60)*1.2 +5;
  //  int minimum = 20; //P=5s
 /* if(in<60) return minimum;
  if(in>=60&&in <80) return minimum + (minimum* ((double) in-60)/20);// 2*minimum at 80; P=2.5)
  if(in>=80&& in <110) return minimum*2 + (minimum* 2*((double) in-(double)80)/(double)30);
  if(in>=110) return minimum*4;*/
}


void setup() {
 Serial.begin(115200);
 
 pinMode(LED_BUILTIN,OUTPUT);
 digitalWrite(LED_BUILTIN, LOW);
 pinMode(12, OUTPUT);

  for(int i = 0; i<clientID; i++){
    digitalWrite(12, HIGH);
    delay(200);
    digitalWrite(12, LOW);
    delay(200);
  }

  pinMode(A0,INPUT);
}


 int startTimeOfNextVibration=2000;

long startTime = millis();
 
void loop()
{
 
  turnOffVibration();
  bool resetTime = false;
  while(true)
  {
    int myBPM =0;
    bool isBPMCoherentEnoughForBeingShared = false;
    senseBPM(myBPM, isBPMCoherentEnoughForBeingShared, resetTime);
  int dataFromServer=0;
    
  //yield();
   dataFromServer=updateDataWithServer(
    clientID,
    otherClientID,
    myBPM,
    startTimeOfNextVibration,
    resetTime);

/*    Serial.print("data from server:");
    Serial.println(dataFromServer);
  */  
    updateVibration(extrapolateDataFromServer(dataFromServer));

 /* if(dataFromServer>0 &&
 !isOutOfSync)
    startTimeOfNextVibration= updateVibration(extrapolateDataFromServer(dataFromServer));
  else
  {
    //Serial.println("No server");
    turnOffVibration();
  }*/
  }
}



