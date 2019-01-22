#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);

const byte rxAddr[6] = "00001";

void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  
  radio.stopListening();

   
}

void loop()
{
  Serial.println("Try so send hello world");
  const char text[] = "Hello World";
  Serial.println(
  radio.write(&text, sizeof(text)));
  
  delay(1000);
}
