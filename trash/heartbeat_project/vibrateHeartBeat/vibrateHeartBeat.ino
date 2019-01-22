const int VIBRATION_PIN = 2;
const int VIBRATION_TIME = 30;//milliseconds

int heartbeatPacePerMinute = 120;
long timeNextVibration = millis();
long timeEndCurrentVibration = millis();

void setup() {
  pinMode(VIBRATION_PIN, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {

  if(millis() > timeNextVibration)
  {
    //Serial.println("update!");
    timeEndCurrentVibration = millis() + VIBRATION_TIME;
    timeNextVibration = millis() + getPeriodFromHBPace();
    digitalWrite(VIBRATION_PIN, HIGH);
  }
  if(millis() > timeEndCurrentVibration)
  {
        digitalWrite(VIBRATION_PIN, LOW);   
       // Serial.println("LOW!");
  }
 /* Serial.print(millis());
  Serial.print(" ");
  Serial.print(timeEndCurrentVibration);
  Serial.print(" ");
  Serial.print(timeNextVibration);
  Serial.println();*/
}

int getPeriodFromHBPace()
{
  return 60000/(1.1*heartbeatPacePerMinute);//fixing delays
}

