#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    20
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100




void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );


  Serial.begin(115200);
}

int current = 0;
    void loop() {
            delay(500); 

      Serial.println(current);
      leds[current]= CRGB::Red; 
      if(current > 0)
      {
              delay(30);
      leds[current - 1]= CRGB::Black;
      }
      delay(30);
      FastLED.show();
      current ++; 
      current = current % NUM_LEDS;
    }

