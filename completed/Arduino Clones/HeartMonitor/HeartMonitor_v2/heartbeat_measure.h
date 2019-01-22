bool wasSignalCoherentLastTime=false;
long expectedPeakHeight = 0;
long nbBeatsSinceLastReset = 0;
long timeStartLastReset=millis();
long timeStartCoherentSignal=millis();
bool wasSignalIncoherentLastRound=false;

long timeLastTimeSignalWasCoherentEnoughForUpdatingBPM = -100000;

long lastTimeValidBeatingWasCounted = millis();
long timeForSensingNextInput = millis();

int lastCoherentBPM=-1;


bool hasSignalBeenCoherentForLongEnoughForUpdatingBPM()
{
  return timeStartCoherentSignal + 10000 < millis();
}

bool hasSignalBeenIncoherentForTooLong()
{
  return (millis() - timeLastTimeSignalWasCoherentEnoughForUpdatingBPM > 60000 );
}

void setSignalAsIncoherent()
{
    wasSignalCoherentLastTime = false;
    timeStartCoherentSignal = millis();
    nbBeatsSinceLastReset = 0;
    timeStartLastReset=millis();
    
}


int getBPMSinceLastReset()
{
  if((millis() - timeStartLastReset)/ 1000 == 0)
    return 0;
  return (nbBeatsSinceLastReset*60*1000) / (millis() - timeStartLastReset);
}



bool isHigh(int x)
{
  return x>expectedPeakHeight*0.90 && x>100;
}

void checkCoherence()
{
  if(millis()-lastTimeValidBeatingWasCounted > 1500)
  {
    setSignalAsIncoherent();
  }
  
}

bool lastTimeWasHigh = false;

void senseBPM(int& lastStableBPM, bool& isBPMCoherentEnoughForBeingShared, bool reset){
  isBPMCoherentEnoughForBeingShared = 
  !hasSignalBeenIncoherentForTooLong();
  
  lastStableBPM = lastCoherentBPM;


  if(reset)
  {
     setSignalAsIncoherent();
     lastCoherentBPM = -1;
     Serial.println(200);
  }


  if(timeForSensingNextInput > millis()) return;
  
  timeForSensingNextInput = millis()+10;

  //expected the peak height decreases as time passes, in case the peak height decreases over time
  expectedPeakHeight *=0.999;
  int x = analogRead(A0)-500;    
  if(x>expectedPeakHeight)
    expectedPeakHeight = x;


   
   Serial.print(isBPMCoherentEnoughForBeingShared*120);
   Serial.print(" ");
   Serial.print(getBPMSinceLastReset());
   Serial.print(" ");
   Serial.print(lastCoherentBPM);
   Serial.print(" ");
   
   
   if(hasSignalBeenCoherentForLongEnoughForUpdatingBPM())
     Serial.print("110 ");
     else Serial.print(min(110,((millis()-timeStartCoherentSignal)/10)));
   Serial.print(" ");

   Serial.print(x);
   Serial.print(" ");
   
   Serial.println(" ");

   if( hasSignalBeenCoherentForLongEnoughForUpdatingBPM()&& 
   millis() - timeStartLastReset > 10000)
   {
     lastCoherentBPM = getBPMSinceLastReset();
     
      timeLastTimeSignalWasCoherentEnoughForUpdatingBPM = millis();
   } 



   checkCoherence();
      
    if(lastTimeWasHigh && !isHigh(x))
    {
          digitalWrite(LED_BUILTIN, HIGH);
          lastTimeWasHigh = false;
    }
    if(!lastTimeWasHigh && isHigh(x))
    {
      if(lastTimeValidBeatingWasCounted+250 > millis())
        setSignalAsIncoherent();

          
          digitalWrite(LED_BUILTIN, LOW);
          lastTimeWasHigh = true;
          lastTimeValidBeatingWasCounted = millis();

          
          
          int bpmAccordingToLastBeatSensed
               = 60000/ (millis() -lastTimeValidBeatingWasCounted);
           
          nbBeatsSinceLastReset++;        
    }
}
