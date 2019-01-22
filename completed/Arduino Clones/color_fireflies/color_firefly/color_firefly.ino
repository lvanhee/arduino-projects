#include <SoftwareSerial.h>


//#include <I2C.h>
SoftwareSerial mySerial(4, 5); // RX, TX
const int stepDuration = 4000;
const long MAX_LIVENESS = 3600*3/(stepDuration/1000);

const byte redPin = 3;
const byte greenPin = 6;
const byte bluePin = 9;

long liveness = 0;



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

void setup() {
  
  Serial.begin(4800);
  Serial.flush();
  mySerial.begin(4800);
  mySerial.flush();

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  testPins();
  randomSeed(analogRead(0));
  do{
  red = 128; //random(3)*127;
  green = 255;//random(3)*127;
  blue = 64;//random(3)*127;
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

void setCurrentColor()
{
  analogWrite(redPin, red*livenessRatio());
  analogWrite(greenPin, green*0.30*livenessRatio());
  analogWrite(bluePin, blue*0.15*livenessRatio());
    Serial.println(green);

}

  void joyAnimation()
  {
      setCurrentColor();
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
  String toSend = "r:"+String(red)+",g:"+String(green)+",b:"+String(blue);
  mySerial.println(toSend);
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

void permanentColorTilt()
{
  Serial.println("Permanent color tilt!!!");
 if(setupRed<=red&&setupRed<255) setupRed++;
        else setupRed--;
  
        if(setupGreen<=green&&setupGreen<255) setupGreen++;
        else setupGreen--;
  
        if(setupBlue<=blue&&setupBlue<255) setupBlue++;
        else setupBlue--;
}

        
void boundAddToLiveness(long val)
{
  liveness+= val;
  if(liveness < 0) liveness = 0;
  if(liveness > MAX_LIVENESS) liveness  = MAX_LIVENESS;
}

long lastTimeReceived = 0;

void receiveFromOther(
  boolean& justReceived,
  boolean& lastReceiveWasLongAgo,
  int& r, int& g, int& b)
{
  justReceived = false;
  lastReceiveWasLongAgo = ((millis()-lastTimeReceived)>10000);

  while(mySerial.available())
  {
    String received = mySerial.readString();
    Serial.print("Received:");
    Serial.println(received);

    if(received.substring(0,2)=="r:"&&received.charAt(received.length()-1)=='\n')
        Serial.println("correct input!");
        else return;

        justReceived = true;

    r = received.substring(2,received.indexOf(",")).toInt();
   /* Serial.print("Red value:");
    Serial.println(r);*/
      
    received =received.substring(received.indexOf(",")+1);

    g = received.substring(2,received.indexOf(",")).toInt();
   /* Serial.print("Green value:");
    Serial.println(g);*/

    received =received.substring(received.indexOf(",")+1);
    b = received.substring(2,received.indexOf("\n")).toInt();
   /* Serial.print("Blue value:");
    Serial.println(b);*/
    
    
    lastTimeReceived = millis();
    
    justReceived = true;

    
    
 /*
    Serial.print(".");
      r = mySerial.parseInt(); 
      g = mySerial.parseInt(); 
      b = mySerial.parseInt(); 
      sendToOther();

      
      newReceived = mySerial.read() == '\n' 
            &&r>0 &&g>0 &&b>0&&r<255&&g<255&&b<255 ;
      Serial.flush();*/
  }
}

boolean isMaxLiveness()
{
  return liveness == MAX_LIVENESS;
}
void updateAsLonely()
{
  Serial.println("Lonely!");
   boundAddToLiveness(-1);          
   tiltColorsTo(setupRed, setupGreen, setupBlue);    
}
void connectWithOtherSelf()
{
  sendToOther();
  int r,g,b;
  boolean justReceived;
  boolean lastReceiveWasLongAgo;
  receiveFromOther(justReceived, lastReceiveWasLongAgo,r,g,b);
  
  if(!justReceived)
  {
    if(lastReceiveWasLongAgo)
    updateAsLonely();  
    
          return;
  }

  if(lastReceiveWasLongAgo && justReceived)
      joyAnimation();  
    
  boundAddToLiveness(MAX_LIVENESS/100);//total refill in 100 iterations = 400s
  
  if(isMaxLiveness())
  {
    Serial.println("Merging with my mate!");
    analogWrite(redPin, r*livenessRatio());
    analogWrite(greenPin, g*0.30*livenessRatio());
    analogWrite(bluePin, b*0.15*livenessRatio());
    delay(200);
    setCurrentColor();
    tiltColorsTo(r, g, b);

    if(abs(r-red) < 3 && abs(b - blue) < 3 && abs(g - green) < 3)
    {
      permanentColorTilt();
      
      analogWrite(redPin, 255*livenessRatio());
      analogWrite(greenPin, 255*0.30*livenessRatio());
      analogWrite(bluePin, 255*0.15*livenessRatio());
      delay(200);
      analogWrite(redPin, setupRed*livenessRatio());
      analogWrite(greenPin, setupGreen*0.30*livenessRatio());
      analogWrite(bluePin, setupBlue*0.15*livenessRatio());
      delay(200);
      setCurrentColor();
    }
  }
}

boolean isDead()
{
  return liveness < 0.001;
}


void loop() {

  Serial.println("Liveness:"+String(liveness)+",("+String(red)+","+String(green)
  +","+String(blue)+
  "),final:("+String(setupRed)+","+String(setupGreen)+","+String(setupBlue)+")");
    delay(stepDuration);

  
  if(isDead())
  {
    delay(1000);
    analogWrite(redPin, red);
    analogWrite(greenPin, green*0.25);
    analogWrite(bluePin, blue*0.25);
    delay(200); 
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0); 
  }
  else
  {
    setCurrentColor();
  }

  connectWithOtherSelf();    
  
}
