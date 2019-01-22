#include <FastLED.h>

#define DATA_PIN 23
#define CLOCK_PIN 18
#define NUM_LEDS    144
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

/**
 * Currently ESP32 only works with APA102 type of LEDs (clock issues)
 */



void setup() {
  delay( 3000 ); // power-up safety delay

LEDS.addLeds<APA102,DATA_PIN, CLOCK_PIN, BGR>(leds,NUM_LEDS).setCorrection(TypicalLEDStrip);;
  FastLED.setBrightness(  BRIGHTNESS );


  Serial.begin(115200);
}

int current = 0;
    void loop() {
            delay(10); 

      Serial.println(current);
      leds[current]= CRGB::Red; 
      if(current > 0)
      {
             // delay(30);
              if(random(2) <0.5)
              leds[current - 1]= CRGB::Black;
      }
      delay(30);
      FastLED.show();
      current ++; 
      current = current % NUM_LEDS;
    }

