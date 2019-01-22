#include <ESP8266WiFi.h>
 

int steps = 10; //number of transitions to go from the old color to the new color
 
 
/*const char* ssid = "SFR-f401";
const char* password = "qhsgzg5kx37c";

const char* ssid = "Livebox-c0cf";
const char* password = "A605780385C805312A08C3F075";*/

const char* host = "api.thingspeak.com";
const char* writeAPIKey = "E12870U6IO7MW22M";
const char* readAPIKey = "E12870U6IO7MW22M";

const int HERMINE_MACHINE = 1;
const int ORNITHO_MACHINE = 2;

int localMachineNumber = ORNITHO_MACHINE;
//1 = hermineside 
//2 = ornithoside

// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80; 

int sensorValue;
int otherSensorValue;
String lastUpdateTime="";

int lastUpdate = millis();

char* getSSID()
{
  if(localMachineNumber==HERMINE_MACHINE)
    return "Livebox-c0cf";
   else return "XXXX-f401";
}

char* getPassword()
{
  if(localMachineNumber==HERMINE_MACHINE)
    return "A605780385C805312A08C3F075";
   else return "qhsgzg5kx37c";
}
 
void setup() {
  //Serial port
  Serial.begin(115200);
  //delay a bit
  delay(100);
 
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(getSSID());
  
  WiFi.begin(getSSID(), getPassword());
  
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  //define LedPin
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level

}
 
//int value = 0;
 
void loop() 
{

  delay(15000);
  printNewLoop();

  readSensor();
  exportSensorValueToServer();
  loadOtherSensorValue();
  updateLighting();
}

void printNewLoop()
{
Serial.println();Serial.println();

  if(localMachineNumber==ORNITHO_MACHINE)
     Serial.println("Loop ornitho-com");
  else 
     Serial.println("Loop hermine-com");

    Serial.print("Local machine number:");
    Serial.println(localMachineNumber);

}

void updateLighting()
{
  if(otherSensorValue>400)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);


}
 void loadOtherSensorValue()
 {
  
    String line = "";
 
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed while loading other sensor value");
    return;
  }

String request = String("GET ") + 
  "/channels/206409/field/"+(3-localMachineNumber)+"/last.xml?results=1&api_key=A0RHP7TVSNGDRCO5"
  //+ url + 
  +" HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n"
               ;

  Serial.println("Loading value from server via command:"+request);
  // This will send the request to the server
  client.print(request);
  delay(100);
 
  
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    
    line = client.readStringUntil('\n');
      if (line.startsWith("  <created-at "))
      {
        if(line==lastUpdateTime)
        {
          Serial.println("Same value as last time");
          if(lastUpdate + 120000 > millis())
          {
             digitalWrite(LED_BUILTIN, LOW);
             Serial.println("Same value for more than 2 minutes");
          }
        }
        else
        {
          Serial.println("New value received!");
           digitalWrite(LED_BUILTIN, HIGH);  
           line = lastUpdateTime;
           lastUpdate = millis();
        }


      }

    if (line.startsWith("  <field")) 
      break;
      //else Serial.println(line);
  }
  line.trim(); //remove spaces 
  int start = line.indexOf('>');
  int last = line.indexOf("</field");
 
  String loadedValue = line.substring(start+1,last);
  if(loadedValue.toInt()!=0)//parfois, ça déconne et on n'a que la valeur 0...
        otherSensorValue = loadedValue.toInt();
        
  Serial.print("Received other sensor value from server:");
  Serial.println(otherSensorValue);
 }
 
void readSensor()
{
    // read the input on analog pin 0:
  sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.print("Loading value of local sensor:");
  Serial.println(sensorValue);
}

 void exportSensorValueToServer()
 {
    WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field";
  url+=localMachineNumber;
  url+="=";
  url+=String(sensorValue);

  url+="\r\n";


  Serial.print("Sending local sensor value via request:");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    delay(1000);

   Serial.println("Just exported sensor value to thingspeak server");

 }
