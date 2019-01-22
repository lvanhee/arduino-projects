#include "C:/Users/lois/Dropbox/Perso/Arduino/Arduino Clones/Acceleration/acceleration/acceleration.ino"

const int PEAK_DISPLAY=0;
const int AVG_DIFF_TO_NO_MOVE=1;

const int OUTPUT_PIN = 9;
void setup() {
//  MPUsetup();
accelerationSetup();
  Serial.begin(9600);
}

void loop() {
  double nextValue =10;// MPUvalue();

  displayUpdate(PEAK_DISPLAY);

 //updater(nextValue);
}

double min = 1000;
double max = -1000;
double fadingFactor = 0.01;
double lastDisplayedValue = 0.001;

void updater(double nextValue)
{
  
  if(nextValue<min)
    min=nextValue;
  if(nextValue>max)
    max = nextValue;

  // min = min + (nextValue - min)*0.001;
    

  lastDisplayedValue = lastDisplayedValue + (nextValue - lastDisplayedValue)*fadingFactor;
    
  int nextDisplayedValue =(lastDisplayedValue-min)/(max-min)* 255;
    
}

void displayUpdate(int mode){
  if(mode == PEAK_DISPLAY)
    peakDisplayUpdate();
  else if(mode == AVG_DIFF_TO_NO_MOVE)
    avgDiffDisplayUpdate();

  analogWrite(OUTPUT_PIN, lastDisplayedValue);


//  Serial.println(lastDisplayedValue);
}

void avgDiffDisplayUpdate(){
  if(isHighChange())
    lastDisplayedValue = lastDisplayedValue + (255 - lastDisplayedValue)*0.001;
  else lastDisplayedValue -=0.05;
  if(lastDisplayedValue < 0) lastDisplayedValue = 0;
    Serial.println  (lastDisplayedValue);

  
}

double stableValue = 0;
void peakDisplayUpdate()
{
  double val = getAcceleration();
  if(max < val) max =val;

  stableValue = stableValue * 0.95 + val*0.05;

  if(val > 0.8* max && val > 1.5 *stableValue)
  {
    lastDisplayedValue = 255;
  }

Serial.print(max);
Serial.print(" ");
Serial.print(val);
Serial.print(" ");
Serial.print(lastDisplayedValue);
Serial.print(" ");
Serial.print(stableValue);
Serial.println();


  lastDisplayedValue *=0.95;
  max = max * 0.999;
}

