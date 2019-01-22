#include <Wire.h>

const long stepDuration =2000;
const long DURATION_FROM_FULL_TO_HIBERNATION = (long)5*60*1000;//s

const long MAX_LIVENESS = 10000;
const long INCREASE_PER_CONNECTED_STEP = 330;//330;  ; // MAX_LIVENESS * stepDuration / DURATION_FROM_FULL_TO_HIBERNATION;
const long DECREASE_PER_LONELY_STEP = -INCREASE_PER_CONNECTED_STEP;

const byte redPin = 3;
const byte greenPin = 6;
const byte bluePin = 9;

long liveness = 0;//MAX_LIVENESS;
bool joy = false;

bool blinkColorOfOther = false;

int colorOfOtherR;
int colorOfOtherG;
int colorOfOtherB;

const bool displaySetupColors = true;
const bool sendSetupColors = true;



int red; int green; int blue;
int setupRed; int setupBlue; int setupGreen;

void  testPins()
{
   analogWrite(redPin, 255);
   delay(200);
   analogWrite(redPin, 0);
   analogWrite(greenPin, 255);
   delay(200);
   analogWrite(greenPin, 0);
   analogWrite(bluePin, 255);
   delay(200);

   analogWrite(bluePin, 0);
   Serial.println("Pins tested");

  //analogWrite(greenPin, green*0.25*livenessRatio());
  //analogWrite(bluePin, blue*0.25*livenessRatio());
}

void setupConnection()
{
      Wire.begin(8);                // join i2c bus with address #8
      Wire.onReceive(receiveEvent); // register event
}
void setup() {

  setupConnection();
  Serial.begin(115200);
  Serial.flush();

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  testPins();
  randomSeed(analogRead(0));
  do{
  red =  random(2)*255;
  green = random(2)*255;
  blue = random(2)*255;
  }while(red==0 && green == 0 && blue ==0);
  setupRed = red;
  setupGreen = green;
  setupBlue = blue;

  Serial.println("Setup done");
  }

  double livenessRatio()
  {
    return (double)liveness/MAX_LIVENESS;
  }

void displayLocalFireflyCurrentColor()
{
  if(displaySetupColors)
  {  
    analogWrite(redPin, setupRed*livenessRatio());
  analogWrite(greenPin, setupGreen*0.30*livenessRatio());
  analogWrite(bluePin, setupBlue*0.15*livenessRatio());}
  else
  { 
    analogWrite(redPin, red*livenessRatio());
    analogWrite(greenPin, green*0.30*livenessRatio());
    analogWrite(bluePin, blue*0.15*livenessRatio());
  }
}

  void joyAnimation()
  {
      displayLocalFireflyCurrentColor();
    for(int i=0; i<15;i++)
      {
        delay(200);
        analogWrite(redPin, random(255));
        analogWrite(greenPin, random(255)*0.25);
        analogWrite(bluePin, random(255)*0.25);
      }
  }
  

void sendToOther()
{
    Wire.beginTransmission(8); // transmit to device #8
    String toSend = "r:"+String(red)+",g:"+String(green)+",b:"+String(blue);
    if(sendSetupColors)
    {
      toSend ="r:"+String(setupRed)+",g:"+String(setupGreen)+",b:"+String(setupBlue);
    }
   // Wire.write("slave is ");        // sends five bytes
    char buffer[32];
    toSend.toCharArray(buffer, 32);
    Wire.write(buffer);
    Wire.endTransmission();    
}

void tiltColorsTo(int r, int g, int b)
{
 if(r>=red&&red<255) red++;
        else red--;
  
        if(g>=green&&green<255) green++;
        else green--;
  
        if(b>=blue&&blue<255) blue++;
        else blue--;
}

void permanentColorTilt(int r, int g, int b)
{
  Serial.println("Permanent color tilt!!!");
 if(setupRed<=r&&setupRed<255) setupRed++;
        else setupRed--;
  
        if(setupGreen<=g&&setupGreen<255) setupGreen++;
        else setupGreen--;
  
        if(setupBlue<=b&&setupBlue<255) setupBlue++;
        else setupBlue--;
}

        
void boundAddToLiveness(long val)
{
  liveness+= val;
  if(liveness < 0) liveness = 0;
  if(liveness > MAX_LIVENESS) liveness  = MAX_LIVENESS;
}

long lastTimeReceived = 0;

boolean isMaxLiveness()
{
  return livenessRatio() > 0.95;
}
void updateAsLonely()
{
  Serial.println("Lonely!");
   boundAddToLiveness(DECREASE_PER_LONELY_STEP);          
   tiltColorsTo(setupRed, setupGreen, setupBlue);    
}
void connectWithOtherSelf()
{
  sendToOther();
}

boolean isDead()
{
  return liveness < 0.001;
}

boolean isLastContactWasLongAgo()
{
  return ((millis()-lastTimeReceived)>10000);
}


void hibernationAnimation()
{
    analogWrite(redPin, red);
    analogWrite(greenPin, green*0.25);
    analogWrite(bluePin, blue*0.25);
    delay(200); 
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0); 
}

int lastFlashHibernation = millis();

void loop() {
  Serial.println("Liveness:"+String(liveness)+",("+String(red)+","+String(green)
  +","+String(blue)+
  "),perm:("+String(setupRed)+","+String(setupGreen)+","+String(setupBlue)+")");
    delay(stepDuration);

    if(isLastContactWasLongAgo())
    {
      updateAsLonely(); 
    }

    if(joy)
    {
      joyAnimation();  
      joy= false;
    }

    if(blinkColorOfOther)
    {
         blinkColorOfOther = false;
         analogWrite(redPin, colorOfOtherR*livenessRatio());
    analogWrite(greenPin, colorOfOtherG*0.30*livenessRatio());
    analogWrite(bluePin, colorOfOtherB*0.15*livenessRatio());

    delay(200);
    displayLocalFireflyCurrentColor();
    }

  
  if(isDead())
  {
    if(millis() - lastFlashHibernation > 1000)
    {
      hibernationAnimation();
      lastFlashHibernation = millis();
    }
  }
  else
  {
    displayLocalFireflyCurrentColor();
  }

  connectWithOtherSelf();    
}


void updateInternalsBasedOnNewData(int receivedR, int receivedG, int receivedB)
{
  if(isLastContactWasLongAgo())
  { joy = true;
  }
  
   lastTimeReceived = millis();


  boundAddToLiveness(INCREASE_PER_CONNECTED_STEP);//total refill in 100 iterations = 400s
  
  if(isMaxLiveness())
  {
    Serial.println("Merging with my mate!");

    for(int i = 0 ; i <3;i++)
    //tiltColorsTo(receivedR, receivedG, receivedB);

     blinkColorOfOther = true;
  colorOfOtherR=receivedR;
  colorOfOtherG=receivedG;
  colorOfOtherB=receivedB;

  //  if(abs(receivedR-red) < 3 && abs(receivedB - blue) < 3 && abs(receivedG - green) < 3)
  //  {
   for(int i = 0 ; i <3;i++)
      permanentColorTilt(receivedR, receivedG, receivedB);
      
    /*   colorOfOtherR=setupRed;
  colorOfOtherG=setupGreen;
  colorOfOtherB=setupBlue;*/
   // }
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {

  String received="";
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    received +=c;
  }

  Serial.println("Received:"+received);


  int receivedR = received.substring(2,received.indexOf(",")).toInt();      
  received =received.substring(received.indexOf(",")+1);

  int receivedG = received.substring(2,received.indexOf(",")).toInt();
  received =received.substring(received.indexOf(",")+1);
  int receivedB = received.substring(2).toInt();
    
  updateInternalsBasedOnNewData(receivedR,receivedG, receivedB);
}
