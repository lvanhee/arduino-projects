int averageBPM=55;
  int avg;
  int averageVariability;
  int recentAverageValues;
  int diffToAvg = 0;
  int nbHighdiffToAvg = 0;

const long SENSING_DURATION=20000;

int nbHighVariabilityUpInARow = 0;

long lastTimeBeatingWasCounted = millis();
long lastTimeSignalWasUnclear = millis();
long lastTimeValidBeatingWasCounted = 0;

bool hasSignalBeenCorrectForLongEnough()
{
  return lastTimeSignalWasUnclear + 10000 < millis();
}
float signalIncoherence = 1;
const int setupTime = millis()+30000;

const int LAST_VALID_BEATS_LENGTH = 20;
long lastValidBeats[LAST_VALID_BEATS_LENGTH];
int lastValidBeatsIndex = 0;

int getValueMeasure()
{
  if(
    millis()<
    lastValidBeats[(lastValidBeatsIndex+1)%LAST_VALID_BEATS_LENGTH]+30000
    //lastTimeValidBeatingWasCounted+10000>millis()
 // &&//hasSignalBeenCorrectForLongEnough()
  //signalIncoherence<0.2
  &&millis() > setupTime
  )
  return averageBPM*1.1;
  else return -1;
}


int turnOffLEDTime = millis();
int lastCheck = millis();
int senseBPM(){
  if(lastCheck > millis()) return getValueMeasure();
  else lastCheck= millis()+10;

  if(millis() > turnOffLEDTime)
    digitalWrite(LED_BUILTIN, HIGH);

    int x = analogRead(A0);
    diffToAvg = abs(x-avg);
    recentAverageValues = recentAverageValues *0.85+x*0.15;

    //depends on the sensor
    //averageVariability= averageVariability*0.99+0.01*diffToAvg;
    averageVariability= averageVariability*0.995+0.005*diffToAvg;

    avg=avg*0.95+x*0.05;
    Serial.print(avg);
    Serial.print(" ");
    Serial.print(avg+averageVariability);
    Serial.print(" ");
    Serial.print(averageBPM);
    Serial.print(" ");
    Serial.print(x);
    Serial.print(" ");
    Serial.print(recentAverageValues);
    Serial.print(" ");
    //Serial.print((millis()-lastTimeValidBeatingWasCounted)/100);
  // Serial.print(" ");
    Serial.print(getValueMeasure());
    Serial.println(" ");

    if(x>avg+max(averageVariability*4,20))
      //maybe new bit detected, wait for confirmation
      nbHighVariabilityUpInARow++;
      
    else
        if (nbHighVariabilityUpInARow>10 && 
        millis()>lastTimeBeatingWasCounted+300)
        {
          //new beat detection confirmed
          Serial.println(00);
          digitalWrite(LED_BUILTIN, LOW);
          turnOffLEDTime=millis()+50;
            nbHighVariabilityUpInARow=0;
            int bpmAccordingToLastBeatSensed
               = 60000/ (millis() -lastTimeBeatingWasCounted);
           
            if(lastTimeBeatingWasCounted+2000>millis())
            {
            //update BPM only if the inut makes sense
              averageBPM= averageBPM*0.9+bpmAccordingToLastBeatSensed*0.1;
              signalIncoherence*=0.9;
              lastTimeValidBeatingWasCounted = millis();
              lastValidBeats[lastValidBeatsIndex]=millis();
              lastValidBeatsIndex=(lastValidBeatsIndex+1)%LAST_VALID_BEATS_LENGTH;
            }
          else 
          {
            lastTimeSignalWasUnclear = millis();
            signalIncoherence += (1-signalIncoherence)*0.05;
          }
        lastTimeBeatingWasCounted = millis();
            
            
          
        }
        else 
        {
        nbHighVariabilityUpInARow=0;   
        if( lastTimeBeatingWasCounted+200>millis())
          signalIncoherence += (1-signalIncoherence)*0.001;
        }

       return getValueMeasure();
}
