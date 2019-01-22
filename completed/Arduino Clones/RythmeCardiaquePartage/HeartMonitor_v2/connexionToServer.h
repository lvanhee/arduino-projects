#include <ESP8266WiFi.h>

#define DURATION_SEND_CYCLE 20000
//const char* ssid = "CLONES";
const char* ssid = "dept-info";
 WiFiClient      TKDClient;
  int             TKDServerPort  = 9001;
  //IPAddress       TKDServer(192,168,0,250);
  IPAddress       TKDServer(192,168,153,250);

bool isConnectingToAP;
  
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

bool isConnectedToAP()
{
  return WiFi.status() == WL_CONNECTED;
}

  void initiateConnectionToAP(int clientID)
  {
    //IPAddress ip(192,168,0,200+clientID);
    IPAddress ip(192,168,153,100+clientID);
    
    IPAddress gateway(192,168,0,1); 
    IPAddress subnet(255,255,255,0); 
    WiFi.config(ip, gateway, subnet);
    
    //WiFi.begin(ssid, password);
    WiFi.begin(ssid);
    
  isConnectingToAP=true;
  }

  boolean isConnectedToServer()
  {
      return TKDClient.connected();
  }

  void connectToServer()
{
 /*TKDClient.stop();

    // If Sucessfully Connected Send Connection Message
    while(! */
    TKDClient.connect(TKDServer, TKDServerPort);
   /* )
    {
      Serial.print("!");
    }
    */
}

void sendDataAndRequestToServer(int clientID,
int myValue,
int otherID
)
{
  
  TKDClient.println 
  (
    String(clientID)+
    ","+
    String(myValue)+
    ","+
    String(otherID)
  );
}

int receiveDataFromServer()
{
  int otherValue = -2;
  while(TKDClient.available())
       {
          String message = TKDClient.readStringUntil('\r');
          Serial.println("Received message:"+message);
          TKDClient.flush();
          if(message.equals("Disconnected"))
            otherValue = -1;
          else otherValue = message.toInt();
       }
   return otherValue;
}

int nextTimeToAcquireData = millis();
bool isTimeToUpdateData()
{
  return millis() > nextTimeToAcquireData;
}

bool hasReceivedDataFromServer()
{
  return TKDClient.available();
}


int dataOfOther = -1;
bool isWifiAwaken = false;
int timeAtWhichTheRequestHasBeenSent;
bool hasSentUpdateAndRequestedData=false;
bool isConnectingToServer= true;

bool isOutOfSync = false;

void resetSending()
{
  nextTimeToAcquireData = millis()+DURATION_SEND_CYCLE;
  hasSentUpdateAndRequestedData = false;
  TKDClient.stop();
  isConnectingToServer= true;
  isOutOfSync = false;
}

int updateDataWithServer
(int myID, int otherID,int myValue, 
  int timeBeforeCriticalPath)
{
  if(timeBeforeCriticalPath<500) return dataOfOther;
  
     yield();
  if(isTimeToUpdateData())
  {
    if(nextTimeToAcquireData+DURATION_SEND_CYCLE*3<millis())
    {
      resetSending();
      isOutOfSync = true;
    }
    if(!isWifiAwaken)
    {
      WiFi.forceSleepWake();
      isWifiAwaken = true;
    }

    if( !isConnectingToAP&& !isConnectedToAP())
    {
       initiateConnectionToAP(myID);
           Serial.println("Updating");

    }

    if(isConnectingToAP&& !isConnectedToAP())
    {
      return dataOfOther;
    }

    

    if(isConnectingToAP)
    {
      isConnectingToAP = false;
      printWifiStatus();
    }
     

    
    while(!isConnectedToServer())
    {
      connectToServer();
      return dataOfOther;
    }

    if(!hasSentUpdateAndRequestedData)
    {

        sendDataAndRequestToServer(myID, myValue, otherID);
        timeAtWhichTheRequestHasBeenSent = millis();
        hasSentUpdateAndRequestedData=true;
        Serial.println("Warned server");
    }


  /* if(hasSentUpdateAndRequestedData && !hasReceivedDataFromServer())
    {
      return dataOfOther;
    }*/
yield();

   /* while(!hasReceivedDataFromServer())
      yield();*/
  Serial.println("Receiving data from server");
    int temp = receiveDataFromServer();
    if(temp==-2) return dataOfOther;
    else dataOfOther = temp;
        
    WiFi.disconnect();
    WiFi.forceSleepBegin();
   isWifiAwaken=false;

  resetSending();
  }
  return dataOfOther;
}

