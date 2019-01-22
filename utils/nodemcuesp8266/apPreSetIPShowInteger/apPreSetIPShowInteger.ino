/*
 * NodeMCU/ESP8266 act as AP (Access Point) and simplest Web Server
 * to control GPIO (on-board LED)
 * Connect to AP "arduino-er", password = "password"
 * Open browser, visit 192.168.4.1
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <WiFiUDP.h>

const char *ssid = "arduino-er";
const char *password = "password";
int numberToDisplay = 0;
WiFiUDP udp;


ESP8266WebServer server(80);

void handleRoot() {
    response();
}

const String HtmlHtml = "<html><head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /></head>";
const String HtmlHtmlClose = "</html>";
const String HtmlTitle = "<h1>Heartbeat AP"+ String(ssid)+" WebServer</h1><br/>\n";

void response(){
  String htmlRes = HtmlHtml + HtmlTitle;

  htmlRes += "<big>Heartbeat is now <b>"+String(numberToDisplay)+"</b></big><br/>\n";

  htmlRes += HtmlHtmlClose;

  server.send(200, "text/html", htmlRes);
}

void establishConnexion()
{
  /*Code pour tranformer l'ESP en AP
   * WiFi.mode(WIFI_AP_STA);  //need both to serve the webpage and take commands via tcp
    IPAddress ip(1,2,3,220);
    IPAddress gateway(1,2,3,1); 
    IPAddress subnet(255,255,255,0); 
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(ssid, password);*/

    WiFi.begin(ssid, password);
    Serial.println("");
    Serial.println("Connecting to WiFi");

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
    
     connectUDP();
}

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);
    
    establishConnexion();

    IPAddress apip = WiFi.softAPIP();
    Serial.print("visit: \n");
    Serial.println(apip); 
    server.on("/", handleRoot);
    server.begin();
    Serial.println("HTTP server beginned");

   
    
    }


void loop() {
    server.handleClient();
    numberToDisplay++;
    digitalWrite(LED_BUILTIN,HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);

    WiFi.forceSleepBegin();
    delay(1000);

    WiFi.forceSleepWake();
    establishConnexion();
     delay(1000);

    IPAddress broadcastIP(192,168,1,41);
    udp.beginPacketMulticast(broadcastIP, 8888, WiFi.softAPIP());
    udp.write(numberToDisplay);
    udp.endPacket();

    Serial.print("Broadcasted packet:");
    Serial.println(numberToDisplay);
}

void connectUDP(){

Serial.println("");
Serial.println("Connecting to UDP");

while(!udp.begin(8888) == 1){
  Serial.println("Connecting to UDP");
}
Serial.println("Connection successful");
}




/*
Serial.begin(115200);
  Serial1.begin(19200);
  WiFi.mode(WIFI_AP_STA);  //need both to serve the webpage and take commands via tcp
  IPAddress ip(1,2,3,4);
  IPAddress gateway(1,2,3,1); 
  IPAddress subnet(255,255,255,0); 
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP("ESPname1"); //Access point is open

  delay(1000);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP); - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=12124#sthash.e8ehrLE6.dpuf

*/
