#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

// wifi connection variables
const char* ssid = "XXXX-f401";
const char* password = "qhsgzg5kx37c";

// UDP variables
unsigned int localPort = 8888;
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back
  IPAddress ip = IPAddress(192,168,0,15);
  IPAddress gateway = IPAddress(192,168,0,1);
  IPAddress subnet = IPAddress(255,255,255,0);

// connect to wifi – returns true if successful or false if not
void connectWifi(){
WiFi.config(ip,gateway,subnet);
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

// Initialise wifi connection
connectWifi();

udpConnected = connectUDP();
}


long lastBlink = millis() ;

void loop() {
  if(millis()>lastBlink+100)
    digitalWrite(LED_BUILTIN,LOW);
  else
    digitalWrite(LED_BUILTIN,HIGH);

// check if the WiFi and UDP connections were successful
if(udpConnected){

// if there’s data available, read a packet
int packetSize = UDP.parsePacket();
if(packetSize)
{
  lastBlink = millis();
Serial.println("");
Serial.print("Received packet of size ");
Serial.println(packetSize);
Serial.print("From ");
IPAddress remote = UDP.remoteIP();
for (int i =0; i < 4; i++)
{
Serial.print(remote[i], DEC);
if (i < 3)
{
Serial.print(".");
}
}
Serial.print(", port ");
Serial.println(UDP.remotePort());

// read the packet into packetBufffer
UDP.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
Serial.println("Contents:");
Serial.println((int) packetBuffer[0]);
int value = packetBuffer[0]*256 + packetBuffer[1];
Serial.println(value);

// send a reply, to the IP address and port that sent us the packet we received
UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
UDP.write(ReplyBuffer);
UDP.endPacket();

// turn LED on or off depending on value recieved
digitalWrite(LED_BUILTIN,value);
}
delay(10);

}
}




// connect to UDP – returns true if successful or false if not
boolean connectUDP(){
boolean state = false;

Serial.println("");
Serial.println("Connecting to UDP");

if(UDP.begin(localPort) == 1){
Serial.println("Connection successful");
state = true;
}
else{
Serial.println("Connection failed");
}

return state;
}
