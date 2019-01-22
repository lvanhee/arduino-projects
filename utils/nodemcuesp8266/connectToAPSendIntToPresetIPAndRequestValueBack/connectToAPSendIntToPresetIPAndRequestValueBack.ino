/*
  WiFi UDP Send and Receive String

 This sketch wait an UDP packet on localPort using a WiFi shield.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort

 Circuit:
 * WiFi shield attached

 created 30 December 2012
 by dlf (Metodo2 srl)

 */



#include <ESP8266WiFi.h>

const char* ssid = "Bbox-81524722";
const char* password = "F6CAFA1CF1F37DF13A722DE453DCCC";
//const char* ssid = "dept-info";
//const char* password = "";

  WiFiClient      TKDClient;
  int             TKDServerPort  = 9001;
  IPAddress       TKDServer(192,168,1,45);

void connectToServer()
{
  Serial.println("Connecting to server");
  TKDClient.stop();

    while(! TKDClient.connect(TKDServer, TKDServerPort))
    {
      delay(100);
      Serial.print("!");
    }
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

/* IPAddress ip(192, 168, 0, 50); // this 3 lines for a fix IP-address
IPAddress gateway(192, 168, 0, 110);
IPAddress subnet(255, 255, 255, 0);
WiFi.config(ip, gateway, subnet);
*/
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  printWifiStatus();

   connectToServer();
}

int clientID = 2;
int myValue = 60;
int otherID = clientID+1%8;

int otherValue = -1;


void loop() {
  while(!TKDClient.connected())
  {
    connectToServer();
  }
  
  TKDClient.println 
  (
    String(clientID)+
    ","+
    String(myValue)+
    ","+
    String(otherID)
  );
  
 delay(20000);
 while(TKDClient.available())
       {
          String Message = TKDClient.readStringUntil('\r');
          TKDClient.flush();
          if(Message.equals("Disconnected"))
            otherValue = -1;
          else otherValue = Message.toInt();
          Serial.println("Other value:"+String(otherValue));
       }
    
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
