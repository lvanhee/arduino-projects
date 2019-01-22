#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

int serial_putc( char c, FILE * ) 
{
  Serial.write( c );

  return c;
} 

void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(9,10);

//
// Topology
//

const uint64_t receiverAddress = 0x0000000000000001LL;


void setup(void)
{

    Serial.begin(57600);
    analogReference(INTERNAL);
  
  radio.begin();
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15); 
  radio.openWritingPipe(receiverAddress);
  //
  // Start listening
  //
  radio.stopListening();
}

void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //      


    // First, stop listening so we can talk.
    

    // Take the time, and send it.  This will block until complete
    unsigned long time = millis();
    
    bool ok = radio.write( &time, sizeof(unsigned long) );
    Serial.print("wrote ");
    Serial.print(ok);
    Serial.print(" ");
    Serial.println(time);
    delay(1000);
}



