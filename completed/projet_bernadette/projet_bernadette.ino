#include <FastLED.h>
#include <Wire.h>
#include "RTClib.h"

//LED strip definition
#define LED_PIN     5
#define NUM_LEDS_TYPES    21
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS_MOON 16

#define AVERAGE_TIME_BETWEEN_TWO_NEW_MOONS 2551442

#define SCHEDULE_START_DAY 17658


DateTime RECORDED_NEW_MOON = DateTime(2018, 01, 17, 03, 18, 18);

CRGB leds[NUM_LEDS_TYPES+NUM_LEDS_MOON];
RTC_DS1307 RTC;

enum MoonCycleLEDs{
  RIGHT, MID_RIGHT,MID_LEFT,LEFT
};

enum TypeDuJour {
  FRUIT,
  FEUILLE,
  RACINE,
  FLEUR,
  NOEUD
};

TypeDuJour typeDuJour[365] =
{
  //AOUT
  FEUILLE,
  FEUILLE,
  FEUILLE,
  FRUIT,
  RACINE,
  RACINE,
  RACINE,
  FLEUR,
  FLEUR,
  NOEUD,
  FRUIT,
  FRUIT,
  FRUIT,
  RACINE,
  RACINE,
  FLEUR,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FRUIT,
  FRUIT,
  NOEUD,
  NOEUD,
  RACINE,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FEUILLE,
  FEUILLE,
  //SEPTEMBRE
  FRUIT,
  RACINE,
  RACINE,
  RACINE,
  FLEUR,
  NOEUD,
  NOEUD,
  FRUIT,
  FRUIT,
  RACINE,
  RACINE,
  RACINE,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FRUIT,
  FRUIT,
  FRUIT,
  NOEUD,
  RACINE,
  RACINE,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FEUILLE,
  FRUIT,
  RACINE,
  RACINE,
  //OCTOBRE
  RACINE,
  FLEUR,
  FLEUR,
  NOEUD,
  NOEUD,
  FRUIT,
  FRUIT,
  RACINE,
  RACINE,
  RACINE,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FRUIT,
  FRUIT,
  NOEUD,
  RACINE,
  RACINE,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FEUILLE,
  FEUILLE,
  FRUIT,
  RACINE,
  RACINE,
  RACINE,
  FLEUR,
  NOEUD,
  //NOVEMBRE
  FEUILLE,
  FRUIT,
  FRUIT,
  RACINE,
  RACINE,
  RACINE,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FRUIT,
  FRUIT,
  NOEUD,
  NOEUD,
  RACINE,
  FLEUR,
  FLEUR,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FRUIT,
  RACINE,
  RACINE,
  RACINE,
  NOEUD,
  NOEUD,
  FEUILLE,
  FRUIT,
  FRUIT,
  //DECEMBRE
  RACINE,
  RACINE,
  RACINE,
  RACINE,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FRUIT,
  FRUIT,
  NOEUD,
  RACINE,
  NOEUD,
  FLEUR,
  FLEUR,
  FLEUR,
  FEUILLE,
  FEUILLE,
  FEUILLE,
  FEUILLE,
  RACINE,
  RACINE,
  RACINE,
  FLEUR,
  NOEUD,
  FEUILLE,
  FRUIT,
  FRUIT,
  FRUIT,
  RACINE,
  RACINE,
  RACINE,
};

void displayMoonCycleBasedOnRatio(int ratioProgressBetweenTwoMoons)
{    
  if(ratioProgressBetweenTwoMoons < 50)
  {
    setMoonLight(RIGHT, constrain(map(ratioProgressBetweenTwoMoons, 0,25,0,255),0,255));
    setMoonLight(MID_RIGHT, constrain(map(ratioProgressBetweenTwoMoons, 10,40,0,255),0,255));
    setMoonLight(MID_LEFT, constrain(map(ratioProgressBetweenTwoMoons, 35,50,0,255),0,255));
    setMoonLight(LEFT, constrain(map(ratioProgressBetweenTwoMoons, 45,50,0,255),0,255));
  }
  else
  {
    setMoonLight(RIGHT, constrain(map(ratioProgressBetweenTwoMoons, 50,55,255,0),0,255));
    setMoonLight(MID_RIGHT, constrain(map(ratioProgressBetweenTwoMoons, 50,65,255,0),0,255));
    setMoonLight(MID_LEFT, constrain(map(ratioProgressBetweenTwoMoons,60,90,255,0),0,255));
    setMoonLight(LEFT, constrain(map(ratioProgressBetweenTwoMoons, 75,100,255,0),0,255));
  }
}

void displayMoonCycle(DateTime now)
{
  DateTime lastNewMoon = RECORDED_NEW_MOON;
  
  while(lastNewMoon.unixtime() < now.unixtime())
  {
    DateTime tmp = DateTime(lastNewMoon.unixtime()+AVERAGE_TIME_BETWEEN_TWO_NEW_MOONS);
    if(tmp.unixtime() >= now.unixtime())
      break;
    lastNewMoon= tmp;
  }
  
  Serial.println("Recorded new moon:");
   Serial.println(String(lastNewMoon.day())+"/"+String(lastNewMoon.month())+
  "/"+String(lastNewMoon.year())+" "+String(lastNewMoon.hour())+":"+String(lastNewMoon.minute())+":"+String(lastNewMoon.second())
  );

  
  //the progress is marked between 0 and 100
  int ratioProgressBetweenTwoMoons = 
  (now.unixtime()*100 - lastNewMoon.unixtime()*100) / AVERAGE_TIME_BETWEEN_TWO_NEW_MOONS;
  Serial.println("Ratio progress between two new moons:"+String(ratioProgressBetweenTwoMoons));

  for(int i = 0 ; i < ratioProgressBetweenTwoMoons ; i ++)
  {
    displayMoonCycleBasedOnRatio(i);
    delay(40);
  }
}


void errorMode()
{
      while(true){
      for(int i = 0; i <NUM_LEDS_TYPES ; i++)
        leds[i] = CRGB::White;
      FastLED.show(); 
      delay(1000);

       for(int i = 0; i <NUM_LEDS_TYPES ; i++)
        leds[i] = CRGB::Black;
      FastLED.show(); 
      delay(1000);
  }
}
bool rtcOffline = false;

void setupRTC()
{
  Wire.begin();
  Serial.println("Wire.begin ok");
  RTC.begin();
  Serial.println("RTC.begin ok");

  if (! RTC.isrunning()) {
    rtcOffline = true;
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));

    errorMode();
  }
}

int dow(long t)
{
    return ((t / 86400) + 3) % 7;
}

void resetLEDs()
{
    for(int i = 0; i <NUM_LEDS_TYPES + NUM_LEDS_MOON; i++)
        leds[i] = CRGB::Black;
        delay(30);
      FastLED.show(); 
      delay(30);
}
void setup() {
  Serial.begin(115200);
  Serial.println("Setup start");
  delay( 3000 ); // power-up safety delay
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS_TYPES+NUM_LEDS_MOON).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  resetLEDs();
  Serial.println("Setup LEDs ok");

  setupRTC();
  Serial.println("Setup RTC ok");

  DateTime now = RTC.now(); 
  displayMoonCycle(now);

  int day = dow(now.unixtime());
  int currentDay = now.unixtime()/3600/24;
  Serial.print("current day:");
  Serial.println(currentDay+","+String(now.day())+"/"+String(now.month())+
  "/"+String(now.year()));
  Serial.println(currentDay);
  
  Serial.println("Day of the week:"+String(day));
  
  int currentDayOnSchedule = currentDay - SCHEDULE_START_DAY;
  for(int i = day; i <NUM_LEDS_TYPES ; i++)
  {
    Serial.print(i);
    Serial.print(":");
    Serial.print(currentDayOnSchedule+i);
    Serial.print(":");
    Serial.println(typeDuJour [currentDayOnSchedule+i]);
    leds[i] = toRGB(typeDuJour [currentDayOnSchedule+i]);
    FastLED.show(); 
    delay(200);
  }
  FastLED.show(); 
}

int b = 0;
void loop() {
}

CRGB toRGB(TypeDuJour t)
{
  switch(t)
  {
    case FRUIT: return CRGB::Red;
    case FLEUR: return CRGB::Yellow;
    case FEUILLE: return CRGB::Green;
    case RACINE: return CRGB::Blue;
    case NOEUD: return CRGB::White;
  }
  return CRGB::Black;
}

void setMoonLight(MoonCycleLEDs mcp, int value)
{
  int start;
  int ending;
  
  switch(mcp)
  {
    case RIGHT:start = 0; ending = 3;break;
    case MID_RIGHT:start = 4; ending = 7;break;
    case MID_LEFT:start = 8; ending = 11;break;
    case LEFT:start = 12; ending = 15;break;
    break;
    default: Serial.println("Error undefined setMoonLight");
  }

  for(int i = start; i <= ending; i ++)
  {
    leds[i+NUM_LEDS_TYPES] = CRGB(value, value, value);
  }
   FastLED.show(); 
}
