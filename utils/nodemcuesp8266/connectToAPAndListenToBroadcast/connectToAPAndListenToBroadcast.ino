#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

// wifi connection variables
const char* ssid = "arduino-er";
const char* password = "password";
unsigned int localPort = 8888;

WiFiUDP UDP;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back


void connectWifi(){

WiFi.begin(ssid, password);
Serial.println("");
Serial.println("Connecting to WiFi");
Serial.println(ssid);
Serial.println(password);

// Wait for connection
Serial.print("Connecting");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}

Serial.println("");
Serial.print("Connected to ");
Serial.println(ssid);
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
}


void setup() {
// Initialise Serial connection
Serial.begin(115200);
pinMode(LED_BUILTIN,OUTPUT);

digitalWrite(LED_BUILTIN,LOW);


resetConnection();
}

void resetConnection()
{
  connectWifi();
  connectUDP();

}

long lastObserved = millis();
int currentVal = 0;
void loop() {
Serial.print("*");

   while( WiFi.status() != WL_CONNECTED)
   {
    Serial.println("Lost connexion to AP");
     resetConnection();
   }

digitalWrite(LED_BUILTIN,HIGH);

// if there’s data available, read a packet
int packetSize = UDP.parsePacket();
if(packetSize>0)
{
   digitalWrite(LED_BUILTIN,LOW); 
  Serial.println();
Serial.print("Received packet of size ");
Serial.println(packetSize);
Serial.print("From ");
IPAddress remote = UDP.remoteIP();
Serial.println(remote);
Serial.print(", port ");
Serial.println(UDP.remotePort());

// read the packet into packetBufffer
UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
Serial.println("Contents:");
Serial.println((int) packetBuffer[0]);
}
delay(500);

}

// connect to UDP – returns true if successful or false if not
void connectUDP(){

Serial.println("");
Serial.println("Connecting to UDP");

while(!UDP.begin(localPort) == 1){
  Serial.println("Connecting to UDP");
}
Serial.println("Connection successful");
}
