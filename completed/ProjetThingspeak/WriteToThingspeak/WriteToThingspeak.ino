#include <ESP8266WiFi.h> // ESP8266WiFi.h library

const char* ssid = "SFR-f401";
const char* password = "qhsgzg5kx37c";
const char* host = "api.thingspeak.com";
const char* writeAPIKey = "E12870U6IO7MW22M";


void setup() {
    Serial.begin(115200);

  // Initialize sensor
  delay(1000);

//  Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

float total = 0;
void loop() {
 total++;

 Serial.println("Go go go!!!");


// make TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  String url = "/update?key=";
  url+=writeAPIKey;
  url+="&field1=";
  url+=String(total);
  url+="\r\n";
  
  // Request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
    delay(1000);
}
