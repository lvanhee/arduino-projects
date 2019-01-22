void setup() {
 pinMode(13,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(A0,INPUT);          // pin that will fade to your heartbeat!
  Serial.begin(115200);             // we agree to talk fast!
  
}

int avg;
int averageVariability;
int diffToAvg = 0;
int nbHighdiffToAvg = 0;
int averageBPM;
int nbBPMsSinceLastReset;
bool lastBeatingIsBeingCounted;

long lastTimeSetup = millis();
void loop() {
  if(millis() > lastTimeSetup+10000)
  {
    averageBPM= averageBPM/2
      +(nbBPMsSinceLastReset*6)/2;

      nbBPMsSinceLastReset = 0;
      lastTimeSetup = millis();
  }
  delay(10);
  int x = analogRead(A0);
  diffToAvg = abs(x-avg);
  averageVariability= averageVariability*0.95+0.05*diffToAvg;
  avg=avg*0.95+x*0.05;
  Serial.print(avg);
  Serial.print(" ");
    Serial.print(avg+averageVariability);
  Serial.print(" ");
    Serial.print(averageBPM);
    Serial.print(" ");
    if(lastBeatingIsBeingCounted)
      Serial.print("1000");
    else Serial.print("0");
  Serial.print(" ");
  Serial.println(x);
  if(x>avg+averageVariability*2)
     {
      if(!lastBeatingIsBeingCounted)
        nbBPMsSinceLastReset++;
      lastBeatingIsBeingCounted = true;
      digitalWrite(13,255);
     }
  else
  {
    digitalWrite(13,0);
    lastBeatingIsBeingCounted = false;
  }
}
