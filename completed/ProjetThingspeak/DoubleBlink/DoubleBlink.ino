const short int BUILTIN_LED1 = 2; //GPIO2
const short int BUILTIN_LED2 = 0;//GPIO16

void setup() {

pinMode(BUILTIN_LED1, OUTPUT); // Initialize the BUILTIN_LED1 pin as an output 
pinMode(BUILTIN_LED2, OUTPUT); // Initialize the BUILTIN_LED2 pin as an output
Serial.begin(115200);
}

void loop() {

  Serial.println("Hi!");

digitalWrite(0, LOW); // Turn the LED ON by making the voltage LOW digitalWrite(BUILTIN_LED2, HIGH); // Turn the LED off by making the voltage HIGH delay(1000); // Wait for a second 
digitalWrite(1, LOW); // Turn the LED ON by making the voltage LOW 
digitalWrite(2, LOW); // Turn the LED ON by making the voltage LOW digitalWrite(BUILTIN_LED2, HIGH); // Turn the LED off by making the voltage HIGH delay(1000); // Wait for a second 
digitalWrite(3, LOW); // Turn the LED ON by making the voltage LOW 
digitalWrite(4, LOW); // Turn the LED ON by making the voltage LOW digitalWrite(BUILTIN_LED2, HIGH); // Turn the LED off by making the voltage HIGH delay(1000); // Wait for a second 
digitalWrite(5, LOW); // Turn the LED ON by making the voltage LOW 
delay(1000); // Wait for two seconds
digitalWrite(BUILTIN_LED1, HIGH); // Turn the LED off by making the voltage HIGH 
digitalWrite(BUILTIN_LED2, HIGH); // Turn the LED off by making the voltage HIGH 
delay(1000); // Wait for two seconds

}
