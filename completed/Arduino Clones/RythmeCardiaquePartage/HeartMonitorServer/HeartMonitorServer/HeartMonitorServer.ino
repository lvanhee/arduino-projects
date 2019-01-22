#include <ESP8266WiFi.h>

const char* ssid = "Sigma-7";
const char* password = "i-Con7";

    #define     NBCLIENTS     12           // MAXIMUM NUMBER OF CLIENTS

    //const char* ssid = "CLONES";
  //  const char* ssid = "dept-info";
  

  int TKDServerPort  = 9001;
  
  WiFiServer  TKDServer(TKDServerPort);      // THE SERVER AND THE PORT NUMBER
  WiFiClient  TKDClient[NBCLIENTS];     // THE SERVER CLIENTS (Devices)


  int memory[NBCLIENTS];
  long lastRefresh[NBCLIENTS];
  long lastCorrectBPM[NBCLIENTS];


/**
 * This function indicates which person/ID a given clientID should sense.
 * Note that multiple clientID can sense the same ID.
 */
int getAllocatedIDFor(int clientID){
  if(clientID==0)return 1;
  if(clientID==1)return 2;
  if(clientID==2)return 1;
  if(clientID==3)return 4;
  if(clientID==4)return 3;
  if(clientID==5)return 6;
  if(clientID==6)return 5;
  if(clientID==7)return 8;
  if(clientID==8)return 7;
  
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

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    for(int i = 0; i < NBCLIENTS; i++)
    {
      memory[i]=-2;
    }


  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

/*  IPAddress ip(192,168,0,250);
  IPAddress gateway(192,168,0,1); */
  IPAddress ip(10,0,0,50);
  IPAddress gateway(10,0,0,1);
  
  IPAddress subnet(255,255,0,0); 

  WiFi.config(ip, gateway, subnet);
    
    WiFi.begin(ssid, password);
 // WiFi.begin(ssid);
       
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("C");
    yield();
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
  return lastRefresh[id] + 120000 >= millis();
}
void checkForNewClients()
{  
  WiFiClient TKDClient = TKDServer.available();

  int startWait = millis();

  
  while(!TKDClient.available())
  {
    Serial.print("|");
    delay(10);
    yield();
    if(millis()>=startWait+DELAYTIME)
    {
      Serial.println("No new client");
      return;
    }
    
    
  }

   Serial.println("New Client");
   while(TKDClient.available())
   {
        yield();
    String Message = TKDClient.readStringUntil('\r');

    int clientID = Message.substring(0,Message.indexOf(",")).toInt();
    Message = Message.substring(Message.indexOf(",")+1);

    int clientValue = Message.substring(0,Message.indexOf(",")).toInt();
    Message = Message.substring(Message.indexOf(",")+1);

    int requestedID = getAllocatedIDFor(clientID);
        //Message.toInt();

    Serial.println("<"+String(clientID)+","+String(clientValue)+","+String(requestedID));

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW

      //Serial.println("test1");

    lastRefresh[clientID] = millis();
    if(clientValue != -1 || (millis()-lastCorrectBPM[clientID] > 120000))
    {
      lastCorrectBPM[clientID] = millis();
      memory[clientID] = clientValue;
    }

    //Serial.println("test2");
    
    if(!isLastRefreshRecentEnough(requestedID))
      {
        Serial.println(">"
        +String(requestedID)
        +"Disconnected");
        TKDClient.print("Disconnected");
      }
    else
    {
      TKDClient.print(String(memory[requestedID]));
      Serial.println(">"
        +String(requestedID)+","
        +String(memory[requestedID]));
        //Serial.println("test3");
    }
    //Serial.println("BeforeFlush");
    TKDClient.flush();
   // Serial.println("AfterFlush");
    }
    //Serial.println("BeforeStop");
    TKDClient.stop();
    //Serial.println("AfterStop");

    displayStatus();
}

void displayStatus()
{
  for(int i = 0 ; i < NBCLIENTS ; i++)
   if(isLastRefreshRecentEnough(i))
     Serial.print(String(i)+":"+String(memory[i])+"\t");
   else
     Serial.print("x\t");
   Serial.println();
}
void loop() {
Serial.print(".");
  checkForNewClients();
  

  yield();
}


