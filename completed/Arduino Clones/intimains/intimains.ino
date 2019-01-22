/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/

const int nbLEDs = 6;
int LEDtoLight=0;
int pins[]={3,5,6,9,10,11};

void slowTurnOn(int pin, int maxV)
{
  for(int L = 0; L < maxV; L++)
      {
        analogWrite (pin, L);
        delay(100);
      }
}


void fastTurnOn(int pin, int maxV)
{
  for(int L = 0; L < maxV; L++)
      {
        analogWrite (pin, L);
        delay(10);
      }
}

void increaseTap(int pin, int maxV)
{
  for(int L = 0; L < maxV; L=L+2)
      {
        analogWrite (pin, L);
        delay(50);
        analogWrite(pin, 0);
        delay(50);
        analogWrite(pin, 0);
      }
    analogWrite (pin, maxV);
}

void decreaseTap(int pin, int maxV)
{
  for(int L = maxV; L > 0; L=L-2)
      {
        analogWrite (pin, L);
        delay(50);
        analogWrite(pin, 0);
        delay(50);
        analogWrite(pin, 0);
      }
    analogWrite (pin, 0);
}

void slowTurnOff(int pin, int maxV)
{
  Serial.println("strokinfoff");
  for(int L = maxV; L>= 0; L--)
      {
        analogWrite (pin, L);
        delay(100);
      }
        Serial.println("strokinfoff-ok");
         analogWrite (pin, LOW);

}

void fastTurnOff(int pin, int maxV)
{
  for(int L = maxV; L>= 0; L--)
      {
        analogWrite (pin, L);
        delay(10);
      }
      analogWrite (pin, LOW);
}

void tapFinger(int part)
{
   increaseTap(part,255);
   decreaseTap(part,255);
}

void strokeFinger(int part)
{
   slowTurnOn(part,255);
   slowTurnOff(part,255);
}

void fastStrokeFinger(int part)
{
   fastTurnOn(part,255);
   fastTurnOff(part,255);
}

void gentleWarmup()
{
      for(int i = 0 ; i < nbLEDs; i++)
    {
       slowTurnOn(pins[i],50);
      delay(5000);
      slowTurnOff(pins[i],50);
    }
}

void fingerPerFinger()
{
  for(int i = 0 ; i < nbLEDs; i++)
    {
      for(int repetition = 0; repetition < 1; repetition ++)
      {
        strokeFinger(pins[i]);
           delay(2000);

      }
      Serial.println("Stroking done");


       for(int repetition = 0; repetition < 1; repetition ++)
      {
        fastStrokeFinger(pins[i]);
         delay(2000);
      }
      for(int repetition = 0; repetition < 1; repetition ++)
      {
        tapFinger(pins[i]);
        delay(2000);
      }
    }
}



void schuffle()
{
  for (int a=0; a<nbLEDs; a++)
  {
   int r = random(a,nbLEDs); // dont remember syntax just now, random from a to 8 included.
   int temp = pins[a];
   pins[a] = pins[r];
   pins[r] = temp;
  }
}

void swapFingers()
{
  int startFinger = pins[random(nbLEDs)];
  int endFinger = pins[random(nbLEDs)];
  while(startFinger == endFinger)
  {
    endFinger = pins[random(nbLEDs)];
  }

  fastTurnOn(startFinger,255);
  
  fastTurnOn(endFinger,255);
  
  fastTurnOff(startFinger,255);
  fastTurnOff(endFinger,255);
}

void addFingers()
{
  schuffle();
  for(int i = 0; i < nbLEDs ; i++)
  {
     fastTurnOn(pins[i],255);
  }

  schuffle();
  for(int i = 0; i < nbLEDs ; i++)
  {
     fastTurnOff(pins[i],255);
  }
}


void fingerSequences()
{
  for(int i = 0; i < 5; i++)
    swapFingers();

  for(int i = 0; i < 5; i++)
    addFingers();
    
}

void flashAll(int nbRepeat)
{
  for(int i =0; i < nbRepeat; i++)
  {
     for(int j = 0; j < nbLEDs ; j++)
      analogWrite (pins[j], 1023);
    delay(100);
   for(int j = 0; j < nbLEDs ; j++)
      analogWrite (pins[j], LOW);
     delay(200);
    
  }
}

void setup() {
  Serial.begin(115200);
    randomSeed(analogRead(A0));

    /* pinMode(11, OUTPUT);
      analogWrite (11, 511);*/

      //return;

    for(int i = 0 ; i < nbLEDs; i++)
    {
      pinMode(pins[i], OUTPUT);
      analogWrite (pins[i], 511);
      delay(500);
      analogWrite (pins[i], LOW);

      
    }

    schuffle();
    delay(2000);
    flashAll(1);
    delay(2000);
    
    gentleWarmup();
    
    delay(2000);
    flashAll(2);
    delay(2000);
    fingerPerFinger();
    
    delay(2000);
    flashAll(3);
    delay(2000);
    fingerSequences();

    while(true)
      flashAll(3);

  }

void loop() 
{
}
