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

int valeur = 0;  // contient la valeur à recevoir 
byte valeur_octet[3]; // contient la valeur découpée en octet pour la réception

void setup() 
{ 
  Mirf.cePin = 8; // CE sur broche 8 
  Mirf.csnPin = 7; // CSN sur broche 7 
  Mirf.spi = &MirfHardwareSpi; // utilisation du port SPI hardware 
  Mirf.init(); // initialise le module SPI 
  Mirf.channel = 0; // utilisation canal 0 pour communiquer (128 canaux disponible, de 0 à 127) 
  Mirf.payload = 3*sizeof( byte); 
  // déclaration taille du message à transmettre, max 32 octets 
  // RF_SETUP=0000abcd : a=1–>2Mb/s, a=0–>1Mb/s; puissance émission bc=00–>-18 dBm, bc=01–>-12dBm, bc=10–>-6dBm, bc=11–>0dBm; 
  // d=0 pas de gain sur le bruit en réception 
  Mirf.configRegister(RF_SETUP, 0x06); // 1 Mb/s et 0 dBm (puissance maximum) 
  Mirf.config(); // configure le canal et la taille du message 
  Mirf.setTADDR((byte *)"ardu1"); // définition adresse sur 5 octets de la 2ème carte Arduino 
  Mirf.setRADDR((byte *)"ardu2"); // définition adresse sur 5 octets de la 1ere carte Arduino 
  
  Serial.begin(115200); 
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() 
{ 
  while(!Mirf.dataReady()) 
  { 
    // pas prêt à recevoir 
    delay(500); 
    Serial.print("."); 
  } 
  Mirf.getData(valeur_octet); // récupére 2 octets 
  Serial.print(valeur_octet[0]); 
   Serial.print(" "); 
  Serial.print(valeur_octet[1]); 
   Serial.print(" "); 
  Serial.println(valeur_octet[2]); 
   delay(500); 
 /* digitalWrite(1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for a second
  digitalWrite(1, LOW);    // turn the LED off by making the voltage LOW
    delay(500);                       // wait for a second*/

}
