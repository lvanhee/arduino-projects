
#include <ESP8266WiFi.h>


    #define     NBCLIENTS     8           // MAXIMUM NUMBER OF CLIENTS

    const char* ssid = "Bbox-81524722";
  const char* password = "F6CAFA1CF1F37DF13A722DE453DCCC";

  int             TKDServerPort  = 9001;
  
  WiFiServer  TKDServer(TKDServerPort);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  TKDClient[NBCLIENTS];     // THE SERVER CLIENTS (Devices)


  int memory[NBCLIENTS];
  int lastRefresh[NBCLIENTS];

  
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

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  

  WiFi.begin(ssid, password);
 /*IPAddress ip(192, 168, 0, 50); // this 3 lines for a fix IP-address
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);*/
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  printWifiStatus();

      TKDServer.begin();
    TKDServer.setNoDelay(true);
    Serial.println("Server Started");
}

int x = 0;

int DELAYTIME = 500;


bool isLastRefreshRecentEnough(int id)
{
  return lastRefresh[id] + 30000 >= millis();
}
void checkForNewClients()
{
  WiFiClient TKDClient = TKDServer.available();

  int startWait = millis();
  while(millis()<startWait+DELAYTIME
  &&!TKDClient.available())
    yield();

   if(millis()>=startWait+DELAYTIME)
   {
   // Serial.println("Crashed client");
    //DELAYTIME++;
    return;
   }
  // else DELAYTIME--;
      
   while(TKDClient.available())
   {
    String Message = TKDClient.readStringUntil('\r');

    int clientID = Message.substring(0,Message.indexOf(",")).toInt();
    Message = Message.substring(Message.indexOf(",")+1);

    int clientValue = Message.substring(0,Message.indexOf(",")).toInt();
    Message = Message.substring(Message.indexOf(",")+1);

    int requestedID =  Message.toInt();

    Serial.println("<"+String(clientID)+","+String(clientValue)+","+String(requestedID));

    lastRefresh[clientID] = millis();
    memory[clientID] = clientValue;
    
    if(!isLastRefreshRecentEnough(requestedID))
      {
        Serial.println(">"
        +String(requestedID)
        +"Disconnected");
        TKDClient.println("Disconnected");
      }
    else
    {
      TKDClient.println(String(memory[requestedID]));
      Serial.println(">"
        +String(requestedID)+","
        +String(memory[requestedID]));
    }
    TKDClient.flush();
    }
    TKDClient.stop();
}

void displayStatus()
{
  for(int i = 0 ; i < NBCLIENTS ; i++)
   if(isLastRefreshRecentEnough(i))
     Serial.print(String(memory[i])+"\t");
   else
     Serial.print("x\t");
   Serial.println();
}
void loop() {

  checkForNewClients();
  displayStatus();
  yield();
}


