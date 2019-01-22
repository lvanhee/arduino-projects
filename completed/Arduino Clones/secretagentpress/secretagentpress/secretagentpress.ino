/* 
    réception d’une valeur integer via module nRF24L01 

    https://itechnofrance.wordpress.com/2013/05/24/utilisation-du-module-nrf24l01-avec-larduino/

    
Broche module  Broche Arduino  Fonction
1 GND Masse
2 3.3v  VCC/3.3v
3 8 (configurable)  CE
4 7 (configurable)  CSN
5 13  SCK
6 11  MOSI
7 12  MISO
8 non connectée IRQ
*/

#include <SPI.h> // gestion du bus SPI 
#include <Mirf.h> // gestion de la communication 
#include <nRF24L01.h> // définition des registres du nRF24L01 
#include <MirfHardwareSpiDriver.h> // communication SPI nRF24L01
#include "LowPower.h"


byte valeur_octet[1]; // contient la valeur découpée en octet pour la réception

const int BUTTON_PIN = 2;

void turnOnMIRF()
{
   Mirf.cePin = 8; // CE sur broche 8 
  Mirf.csnPin = 7; // CSN sur broche 7 
  Mirf.spi = &MirfHardwareSpi; // utilisation du port SPI hardware 
  Mirf.init(); // initialise le module SPI 
  Mirf.channel = 11; // utilisation canal 0 pour communiquer (128 canaux disponible, de 0 à 127) 
  //SECRET CHANNEL!!!
  
  Mirf.payload = sizeof( byte); // = 2, déclaration taille du message à transmettre, max 32 octets 
  // RF_SETUP=0000abcd : a=1–>2Mb/s, a=0–>1Mb/s; puissance émission bc=00–>-18 dBm, bc=01–>-12dBm, bc=10–>-6dBm, bc=11–>0dBm; 
  // d=0 pas de gain sur le bruit en réception 
  Mirf.configRegister(RF_SETUP, 0x06); // 1 Mb/s et 0 dBm (puissance maximum) 
  Mirf.config(); // configure le canal et la taille du message 
  Mirf.setTADDR((byte *)"ardu1"); // définition adresse sur 5 octets de la 2ème carte Arduino 
  Mirf.setRADDR((byte *)"ardu1"); // définition adresse sur 5 octets de la 1ere carte Arduino 
}

void setup() 
{ 
    pinMode(LED_BUILTIN, OUTPUT);                  // wait for a second

 turnOnMIRF();
  
  Serial.begin(115200); 
   pinMode(BUTTON_PIN, INPUT_PULLUP);   

   pinMode(4, OUTPUT);
   digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);
    digitalWrite(4, LOW);   // turn the LED on (HIGH is the voltage level)
   randomSeed(analogRead(0));

    
Mirf.powerDown();
   
}

void sendForTenSeconds()
{
      Serial.println("Sending!");
      long start = millis();
      valeur_octet[0] = random(255);
      int nbRepet =0;
      while(millis() < start + 20000)
      {
       
        Mirf.send(valeur_octet);
        while(Mirf.isSending())
        {
          delay(100);
          Serial.print(".");
        }
        delay(400);
        Serial.println(nbRepet);
        nbRepet++;
      }
}

byte lastReceived =0;
byte justReceived =0;

long lastReset = millis();

void loop() 
{
     LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
     ///also turns off the clock!!!
     delay(100);     
  if(digitalRead(BUTTON_PIN)==1 && lastReset +2000 >millis())
  return;
  
  Serial.println("Loop");
  lastReset =millis();
turnOnMIRF();

if(digitalRead(BUTTON_PIN)!=1) 
    {
       digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);
    digitalWrite(4, LOW);   // turn the LED on (HIGH is the voltage level)
      sendForTenSeconds();
      
    }

    bool sensedData = false;

    delay(1000);
  while(Mirf.dataReady()) 
  {
    Serial.println("Got Data!"); 
    Mirf.getData(valeur_octet); // récupére 2 octets 
    Serial.println((byte)valeur_octet[0]);
    sensedData = true;
    justReceived = (byte)valeur_octet[0];
  } 
   
  if(sensedData &&     lastReceived != justReceived){
    lastReceived = justReceived;
    for(int i = 0; i < 3 ; i++)
    {
    digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(150);
    digitalWrite(4, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(300);
    }
   }


  /*while(!Mirf.dataReady()) 
  { 
    if(true) 
    {
      //Mirf.powerDown();
      delay(2000);
      Serial.println("Sending!");
      Mirf.send(0);
      while (Mirf.isSending())
      {
       delay(100);
       //Serial.print("!");
      }
    }
    // pas prêt à recevoir 
    delay(10); 
    Serial.print("."); 
  } 
   Mirf.getData(valeur_octet); // récupére 2 octets 
       digitalWrite(4, HIGH);   // turn the LED on (HIGH is the voltage level)

   for(int i=0;i<500;i++)
      Serial.println("received!"); 
   delay(200);
   digitalWrite(4, LOW);   // turn the LED on (HIGH is the voltage level)*/

   Mirf.powerDown();

}
