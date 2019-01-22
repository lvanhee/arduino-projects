/**
 * MPU: 
 *  VCC->5V
 *  GND->GND
 *  SCL->A5
 *  SDA->A4
 *  INT->D2
 *  
 * Transistor:
 *  Left -> 9
 *  Mid: White
 *  Right-> GND
 *  Noir -> GND
 */

const int PEAK_DISPLAY = 0;
const int OUTPUT_PIN = 9;
void setup() {
  //  MPUsetup();
  accelerationSetup();
  Serial.begin(9600);
}

void loop() {
  double nextValue = 10; // MPUvalue();

  displayUpdate(getAcceleration(), PEAK_DISPLAY);

  //updater(nextValue);
}

double min = 1000;
double max = -1000;
double fadingFactor = 0.01;
double lastDisplayedValue = 0.001;

void updater(double nextValue)
{

  if (nextValue < min)
    min = nextValue;
  if (nextValue > max)
    max = nextValue;

  // min = min + (nextValue - min)*0.001;


  lastDisplayedValue = lastDisplayedValue + (nextValue - lastDisplayedValue) * fadingFactor;

  int nextDisplayedValue = (lastDisplayedValue - min) / (max - min) * 255;

  Serial.print("Received:");
  Serial.print(nextValue);
  Serial.print("Min:");
  Serial.print(min);

  Serial.print("To display:");
  Serial.println(nextDisplayedValue);
  analogWrite(9, 255);


}

void displayUpdate(double detectedValue, int mode) {
  if (mode == PEAK_DISPLAY)
    peakDisplayUpdate(detectedValue);

  //  Serial.println(lastDisplayedValue);
}

void peakDisplayUpdate(double val)
{

  if (isPeakDetected())
    lastDisplayedValue = 255;
  else
    lastDisplayedValue = 0.98 * lastDisplayedValue + 0.02 * 255 * getRelativeExcitation();

  /*  if(val > 0.8* max && val > 1.5 *stableValue)
    {
      lastDisplayedValue = 255;
    }*/
  Serial.print("last ");
  Serial.print(lastDisplayedValue);
  /*
    Serial.print(max);
    Serial.print(" ");
    Serial.print(val);
    Serial.print(" ");
    Serial.print(lastDisplayedValue);
    Serial.print(" ");
    Serial.print(stableValue);*/
  Serial.println();

  analogWrite(OUTPUT_PIN, lastDisplayedValue);
}

