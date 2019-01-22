const int DURATION_VIBRATION_PER_CYCLE = 200;

int getDurationVibrationCycle(int averageBPM)
{
  return 60000/averageBPM;
}

long lastStartVibration=millis();
bool isVibrating = false;


void turnOffVibration()
{
  digitalWrite(12, LOW);
      isVibrating = false;    
}

int updateVibration(int averageBPM)
{
    
  if(averageBPM<=0)
  {
    turnOffVibration();
    return 10000;
  }

  if(lastStartVibration+getDurationVibrationCycle(averageBPM)<millis())//time for a new cycle
  {
    lastStartVibration = millis();
    isVibrating = true;
    digitalWrite(12, HIGH);
    return 0;
  }
  if(isVibrating && millis() > lastStartVibration + DURATION_VIBRATION_PER_CYCLE)//time for stopping the vibration
  {
    turnOffVibration();
    return getDurationVibrationCycle(averageBPM)+lastStartVibration + DURATION_VIBRATION_PER_CYCLE- millis();
  }
  
  if(millis()
  <  DURATION_VIBRATION_PER_CYCLE+lastStartVibration+1)return 0;
  return getDurationVibrationCycle(averageBPM)+lastStartVibration + DURATION_VIBRATION_PER_CYCLE- millis();
}



