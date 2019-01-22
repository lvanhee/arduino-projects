#include <IRremote.h> // Include the IRremote library

/* Setup constants for SparkFun's IR Remote: */
#define PIN_SWITCH 10

/* Define the IR remote button codes. We're only using the
   least signinficant two bytes of these codes. Each one 
   should actually have 0x10EF in front of it. Find these codes
   by running the IRrecvDump example sketch included with
   the IRremote library.*/
const uint16_t BUTTON_POWER = 0xD827; // i.e. 0x10EFD827
const uint16_t BUTTON_A = 0xF807;
const uint16_t BUTTON_B = 0x7887;
const uint16_t BUTTON_C = 0x58A7;
const uint16_t BUTTON_UP = 0xA05F;
const uint16_t BUTTON_DOWN = 0x00FF;
const uint16_t BUTTON_LEFT = 0x10EF;
const uint16_t BUTTON_RIGHT = 0x807F;
const uint16_t BUTTON_CIRCLE = 0x20DF;

/* Connect the output of the IR receiver diode to pin 11. */
int RECV_PIN = 11;
/* Initialize the irrecv part of the IRremote  library */

IRrecv irrecv(RECV_PIN);
decode_results results; // This will store our IR received codes
uint16_t lastCode = 0; // This keeps track of the last code RX'd

/* Setup RGB LED pins: */
enum ledOrder // Make an enum to add some clarity in the code
{
  RED,   // 0
  GREEN, // 1
  BLUE   // 2
};
const int rgbPins[3] = {5, 9, 6}; // Red, green, blue pins respectively
byte rgbValues[3] = {55, 23, 200}; // This keeps track of channel brightness
byte activeChannel = RED; // Start with RED as the active channel
boolean ledEnable = 1; // Start with the LED on.

void setup()
{
  Serial.begin(115200); // Use serial to debug. 
  irrecv.enableIRIn(); // Start the receiver
  pinMode(13, OUTPUT);
  pinMode(PIN_SWITCH, OUTPUT);
  digitalWrite(PIN_SWITCH, HIGH);

  
        digitalWrite(PIN_SWITCH,LOW);
        delay(50);
        digitalWrite(PIN_SWITCH,HIGH);

  /* Set up the RGB LED pins: */
  for (int i=0; i<3; i++)
  {
    pinMode(rgbPins[i], OUTPUT);
    analogWrite(rgbPins[i], rgbValues[i]);
  }
}

// loop() constantly checks for any received IR codes. At the
// end it updates the RGB LED.
void loop() 
{
  if (irrecv.decode(&results)) 
  {
    /* read the RX'd IR into a 16-bit variable: */
    uint16_t resultCode = (results.value & 0xFFFF);
    
    Serial.println(resultCode);
    irrecv.resume();
    if(resultCode != 11475) return ;

    //peut checker si le code correspond
    
        digitalWrite(PIN_SWITCH,LOW);
        delay(400);
        digitalWrite(PIN_SWITCH,HIGH);
    
  }
}
