const int MAX_DURATION_VIBRATION_PER_CYCLE = 800;

long getVibrationTimePerVibrationCycle(int averageBPM)
{
  if(averageBPM <=0) return -1;
  if(averageBPM >= 80) return 100;
  return 400-(averageBPM-20)*5;
  
  /*if(averageBPM <= 20) return 500;
    if(averageBPM <= 40) return 400;
    if(averageBPM <= 60) return 300;
    if(averageBPM <= 80) return 200;
    if(averageBPM >= 80) return 100;
    
  return MAX_DURATION_VIBRATION_PER_CYCLE/averageBPM;*/
}

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
  if(isVibrating && millis() > lastStartVibration + getVibrationTimePerVibrationCycle(averageBPM))//time for stopping the vibration
  {
    turnOffVibration();
    return getDurationVibrationCycle(averageBPM)+lastStartVibration + getVibrationTimePerVibrationCycle(averageBPM) - millis();
  }
  
  if(millis()
  <  getVibrationTimePerVibrationCycle(averageBPM) +lastStartVibration+1)return 0;
  return getDurationVibrationCycle(averageBPM)+lastStartVibration + getVibrationTimePerVibrationCycle(averageBPM) - millis();
}



