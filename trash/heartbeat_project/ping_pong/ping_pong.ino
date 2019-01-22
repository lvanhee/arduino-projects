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

// connect to wifi – returns true if successful or false if not
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

// Initialise wifi connection
connectWifi();

udpConnected = connectUDP();
}

int currentVal = 0;
void loop() {
  currentVal++;
// check if the WiFi and UDP connections were successful
if(udpConnected){
  byte message[2];
message[0] = currentVal;
message[1] = 1;
UDP.beginPacket(IPAddress(192,168,0,15),localPort);
UDP.write(currentVal);
UDP.endPacket();
Serial.println("Message sent!");
Serial.println((int) message[0]);
}
delay(1000);

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
