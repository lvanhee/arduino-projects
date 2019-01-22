#include <FastLED.h>


#define DATA_PIN 23
#define CLOCK_PIN 18
#define NUM_LEDS    144
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NB_HEADS 10

#define RED_LED_PIN 15
#define GREEN_LED_PIN 16
#define BLUE_LED_PIN 17

CRGB leds[NUM_LEDS];

double headPositions[NB_HEADS];
double headSpeeds[NB_HEADS];
CRGB headColors[NB_HEADS];

CRGB possibleFlash[NUM_LEDS];
long isFlashingSince[NUM_LEDS];
int chanceFlash[NUM_LEDS];

int currentIndex = 0;

void setupDisplay()
{
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  
  LEDS.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);;
  FastLED.setBrightness(  BRIGHTNESS );
   for(int i = 0 ; i < NB_HEADS ; i ++)
  {
    headPositions[i]=-1;
  }

  for(int i = 0 ; i < NUM_LEDS ; i ++)
  {
    chanceFlash[i]=0;
    possibleFlash[i] = CRGB::Black;
  }
}

long timeLastNewBeat = millis();

bool isTimeForNewHeartBeat(int heartRate)
{
  if (heartRate <= 0) return isTimeForNewHeartBeat(60);
  long timeSinceLastBeat = millis() - timeLastNewBeat;
  return timeSinceLastBeat > ((long)60000) / heartRate;
}


double modifiedMap(double x, double in_min, double in_max, double out_min, double out_max)
{
  if(x < in_min) return out_min;
 double temp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 temp = (int) (4*temp + .5);
 return (double) temp/4;
}


void addNewHead(int heartRate,int r, int g, int b)
{
  int maximum = r;
    if(maximum < g) maximum = g;
    if(maximum < b) maximum = b;
    
    r = map(r, 0, maximum, 0, 255);
    g = map(g, 0, maximum, 0, 255);
    b = map(b, 0, maximum, 0, 255);

    if(r < 64) r = 0;
    if(g < 64) g = 0;
    if(b < 64) b = 0;

    headPositions[currentIndex] = 0;

    
    headColors[currentIndex].red = r;
    headColors[currentIndex].green = g;
    headColors[currentIndex].blue = b;
    timeLastNewBeat = millis();
    headSpeeds[currentIndex]= modifiedMap(heartRate, 60,80, 1,2);
    if(headSpeeds[currentIndex] < 1)headSpeeds[currentIndex] = 1;

    Serial.println(headSpeeds[currentIndex]);

    
    currentIndex = (currentIndex + 1) % NB_HEADS;
}

int current = 0;
long lastDisplayUpdate = millis();
void loopDisplay(int heartRate, int r, int g, int b)
{
 // if (millis() < lastDisplayUpdate + 50)
 //  return;
  lastDisplayUpdate = millis();

  if (isTimeForNewHeartBeat(heartRate))
  {
    addNewHead(heartRate, r, g, b);
  }

  for (int i = 0; i < NUM_LEDS   ; i++)
  {
   /* if(chanceFlash[i] > random(100))
    {
      leds[i].red = random(possibleFlash[i].red/2);
      leds[i].green = random(possibleFlash[i].green/2);
      leds[i].blue = random(possibleFlash[i].blue/2);
    }
     else  */
      leds[i].fadeToBlackBy(100);
    //  =CRGB::Black;

      chanceFlash[i]*=0.7;
  }

 for(int i = 0 ; i < NB_HEADS ; i ++)
  {
    if(headPositions[i] < 0 || headPositions[i] >= NUM_LEDS)
      headPositions[i] = -1;
    if(headPositions[i] < 0) continue;
    int pos = (int)headPositions[i];    
    chanceFlash[pos] = 100;
    possibleFlash[pos] = headColors[i];
    leds[pos] = headColors[i];

    headPositions[i]+=headSpeeds[i];

  }
  
  delay(30);
  FastLED.show();
}

