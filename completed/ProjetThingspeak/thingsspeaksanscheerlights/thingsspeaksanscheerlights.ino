#include <ESP8266WiFi.h>
 
//Nepixels
//connected Pin
#define NeopixelPIN 4; 
//Num of Neopixels
#define NumNeopixels 16;
 
int steps = 10; //number of transitions to go from the old color to the new color
 
 
const char* ssid = "SFR-f401";
const char* password = "qhsgzg5kx37c";
const char* host = "api.thingspeak.com";
 
// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 80;
 
// List of CheerLights color names
String colorName[] = {"none","red","pink","green","blue","cyan","white","warmwhite","oldlace","purple","magenta","yellow","orange"};
 
// Map of RGB values for each of the Cheerlight color names
int colorRGB[][3] = {     0,  0,  0,  // "none"
                        255,  0,  0,  // "red"
                        255,79,166,  // "pink"
                          0,255,  0,  // "green"
                          0,  0,255,  // "blue"
                          0,255,255,  // "cyan",
                        255,255,255,  // "white",
                        255,223,223,  // "warmwhite",
                        255,223,223,  // "oldlace",
                        128,  0,128,  // "purple",
                        255,  0,255,  // "magenta",
                        255,255,  0,  // "yellow",
                        255,140,  0}; // "orange"}; 
 
 
//store the oldColor
String oldColor = "";
 
void setup() {
  //Serial port
  Serial.begin(115200);
  //delay a bit
  delay(100);
 
  //Start the Neopixels
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  //define LedPin
  pinMode (4, OUTPUT);
}
 
//int value = 0;
 
void loop() {
 
  String line = "";
  delay(5000);
 
 
  Serial.print("connecting to ");
  Serial.println(host);
  
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/channels/1417/field/1/last.xml";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
 
  
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    
    line = client.readStringUntil('\n');
    if (line.startsWith("  <field1>")) {
      //Serial.print(line);
      //colortobe = line;
      break;
    }
  }
  line.trim(); //remove spaces
//  Serial.print("Line: ");
  //Serial.println(line);
 
  int start = line.indexOf('>');
//  Serial.print("Int: ");
//  Serial.println(start);
  int last = line.indexOf("</field1>");
//  Serial.print("Last: ");
//  Serial.println(last);
 
  Serial.print ("The color is: ");
  String theColor = line.substring(start+1,last);
  Serial.println(theColor);
  setColor(theColor);
//  Serial.println();
//  Serial.println("closing connection");
}
 
 
void setColor(String color) {

  
  int a, b, c;
  int valueA, valueB, valueC;
 
  int curStep = 0;    //current iteration
  
  //get the old color
   for(int iColor = 0; iColor <= 12; iColor++) {
    if(oldColor == colorName[iColor]) {
      a = colorRGB[iColor][0];
      b = colorRGB[iColor][1];
      c = colorRGB[iColor][2];
    }
  }
  //old color
  /*
  Serial.println ("Old Color");
  Serial.print (a);
  Serial.print (" ");  
  Serial.print (b);
  Serial.print (" ");  
  Serial.println (c);
  delay (3000); //to remove
  */
  
  // Look through the list of colors to find the one that was requested
  //valueXYZ is the new color
  for(int iColor = 0; iColor <= 12; iColor++) {
    if(color == colorName[iColor]) {
/*      Serial.print("Color is: ");
      Serial.println(colorName[iColor]);
      Serial.print("iColor is: ");
      Serial.println(iColor); */
      valueA = colorRGB[iColor][0];
      valueB = colorRGB[iColor][1];
      valueC = colorRGB[iColor][2];
      //colorSet(pixels.Color (colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
    }
  }
 
/*
  //New color
  Serial.println ("New Color");
  Serial.print (valueA);
  Serial.print (" ");
  Serial.print (valueB);
  Serial.print (" ");  
  Serial.println (valueC);
  delay (3000); //to remove
*/
  //find the value for the transitions
  int stepA = abs((valueA - a) / steps);
  int stepB = abs((valueB - b) / steps);
  int stepC = abs((valueC - c) / steps);
 
/*
  //steps values
  Serial.println ("step values");
  Serial.print (stepA);
  Serial.print (" ");
  Serial.print (stepB);
  Serial.print (" ");  
  Serial.println (stepC);
  delay (3000); //to remove
 
*/
  //go throw the transitions of colors
  //Serial.println ("Iterations: ");
  while (curStep < steps) {
    a > valueA ? (a -= stepA) : (a += stepA);
    b > valueB ? (b -= stepB) : (b += stepB);
    c > valueC ? (c -= stepC) : (c += stepC);
/*
    //give some info
    Serial.print (a);
    Serial.print (" ");
    Serial.print (b);
    Serial.print (" ");    
    Serial.println (c);
    */
    
    //set the color
    delay(110); //delay a bit so we can observe the change of colors
 
    curStep++;
  }
 
  //final iteration and add the remainder 
  /*
  Serial.print (a);
  Serial.print (b);
  Serial.println (c);*/
  a += (valueA - a);
  b += (valueB - b);
  c += (valueC - c);
  oldColor = color;
  /*
  Serial.print ("old color ");
  Serial.println(oldColor);*/
}
 
/*
void setColor(String color) {
  // Look through the list of colors to find the one that was requested
  for(int iColor = 0; iColor <= 12; iColor++) {
    if(color == colorName[iColor]) {
      colorSet(pixels.Color (colorRGB[iColor][0], colorRGB[iColor][1], colorRGB[iColor][2]));
    }
  }
}
 
*/
// Fill strip with a color
void colorSet(uint32_t c) {
}
