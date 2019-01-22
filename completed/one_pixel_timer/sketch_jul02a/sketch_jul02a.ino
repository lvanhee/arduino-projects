
  const byte redPin = 3;
  const byte greenPin = 6;
  const byte bluePin = 9;


  const long start = millis();
  const long duration = (long)120*60*1000;
  const long ending = start+duration;
  const long end_blink_threshold = duration- (long)5*60*1000;

  
void setup() {
  Serial.begin(115200);
  Serial.flush();

  Serial.println(duration);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

long current = millis();
long last = millis();


void loop() {
  last = current;
  current = millis();
  Serial.print(current);
  Serial.print(" ");
  Serial.print(last);
  Serial.print(" ");
  Serial.print(end_blink_threshold);
  Serial.print(" ");
  Serial.println(ending);
  
  if(millis() < ending)
  {
    double ratio_remaining = 1-((double)millis()-start)/duration;
    analogWrite(redPin, (1-ratio_remaining)*255);
    analogWrite(greenPin, ratio_remaining*255/4);

    if(last <= end_blink_threshold 
        && current >= end_blink_threshold)
    {
      analogWrite(redPin, 255);
      analogWrite(greenPin, 255/4);
      analogWrite(bluePin, 255/4);
      delay(5000);
       analogWrite(bluePin, 0);

    }
  }
  else
  {
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255/4);
    analogWrite(bluePin, 255/4);
    delay(1000);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    delay(1000);
  }

  delay(100);
}
