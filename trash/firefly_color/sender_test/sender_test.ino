/*
    émission d’une valeur integer via module nRF24L01

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

int valeur = 123;  // contient la valeur à envoyer
byte valeur_octet[3]; // contient la valeur découpée en octet pour l’envoi

byte red = 255;
byte green = 255;
byte blue = 255;
byte redPin = 3;
byte greenPin = 5;
byte bluePin = 6;


void setup()
{
  Mirf.cePin = 8; // CE sur broche 8
  Mirf.csnPin = 7; // CSN sur broche 7
  Mirf.spi = &MirfHardwareSpi; // utilisation du port SPI hardware
  Mirf.init(); // initialise le module SPI
  Mirf.channel = 0; // utilisation canal 0 pour communiquer (128 canaux disponible, de 0 à 127)
  Mirf.payload = 3 * sizeof(byte); // = 2, déclaration taille du message à transmettre, max 32 octets
  // RF_SETUP=0000abcd : a=1–>2Mb/s, a=0–>1Mb/s; puissance émission bc=00–>-18 dBm, bc=01–>-12dBm, bc=10–>-6dBm, bc=11–>0dBm;
  // d=0 pas de gain sur le bruit en réception
  Mirf.configRegister(RF_SETUP, 0x06); // 1 Mb/s et 0 dBm (puissance maximum)
  Mirf.config(); // configure le canal et la taille du message
  Mirf.setTADDR((byte *)"ardu2"); // définition adresse sur 5 octets de la 2ème carte Arduino
  Mirf.setRADDR((byte *)"ardu1"); // définition adresse sur 5 octets de la 1ere carte Arduino
  Serial.begin(115200);
    pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  randomSeed(analogRead(0));
   red = random(255);
    green = random(255);
    blue = random(255);

}



int count = 0;
void loop()
{
  if(red == 255) red--;
  if(green == 255) green--;
  if(blue == 255) blue--;

  if(red == 0) red++;
  if(green == 0) green++;
  if(blue == 0) blue++;
  red += random(3)-1;
    green += random(3)-1;
    blue += random(3)-1;
    

  count++;

  valeur_octet[0] = count; // 1er octet
  valeur_octet[1] = count + 1;
  valeur_octet[2] = count + 10;

  setColor(red, green*0.5, blue*0.01);
  Serial.print(blue);
  Serial.print(" ");
  Serial.print(red);
  Serial.print(" ");
  Serial.println(green);

  // Mirf.send(valeur_octet);

  // Serial.println("sending");
/*  while (Mirf.isSending())
  {
    //    Serial.println("try to send");
    //  delay(500);
  }*/
  // Serial.println("sent");
  // Serial.println(valeur_octet[0]);
  delay(10);
}

void setColor(int red, int green, int blue)
{
  analogWrite(greenPin, green);
    analogWrite(redPin, red);

  analogWrite(bluePin, blue);
}
