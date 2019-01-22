#include <Adafruit_SSD1306.h>

const int buttonPin = 2;
const int lightPin = 6;
const int potIn = A0;

#define VIBRATOR_PIN 5
#define SOUND_PIN 7
#define GREEN_LED_PIN 10
#define RED_LED_PIN 11


#define ANALYSIS_DURATION 5000
#define MAX_VIBRATION_PASSIVE_MOD 160

#define DANGER_THRESHOLD 230

long lastUpdatePowerLevel = millis();
long timeStartProcessing = 0;

//the other side of the button should be connected to ground
Adafruit_SSD1306 display(4);

bool dangerMessageIsBeingDisplayed = false;

String currentAnalyzis = "";

bool hasTriggerButtonBeenFired = false;

void setupBouton()
{
    pinMode(SOUND_PIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(lightPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(VIBRATOR_PIN, OUTPUT);


}
long lastPositive = millis()-10000;
long bootupTime = millis();
void setup() {
  //start serial connection
  Serial.begin(115200);

  setupBouton();

  pinMode(potIn, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  lastPositive = millis();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Booting up");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.display();
}

bool isTriggered()
{
  return lastPositive +  100 > millis();
}

void showIsCapturing()
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Capturing!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.display();
}


void showAnalyzing()
{
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Processing...");
  display.display();
}

void showPower(int powerLevel, bool isDanger)
{    
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Power\n"+String(map(powerLevel,0,255,0,100))+"%");
display.display();
}

void showDanger()
{
  display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,0);

    display.setTextSize(3);
    display.println(" DANGER"); 

        
    display.setTextSize(1);
    display.println("     Overcharge"); 
    
    display.display();
}
void showResult()
{
  
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(2);
  if(hasTriggerButtonBeenFired)
    display.println("Result:");
  else 
    display.println("No result");
  display.println(currentAnalyzis);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  
  display.display();
}

bool isCapturingData()
{
  return isTriggered();
}

bool isAnalyzing()
{
  return hasTriggerButtonBeenFired & 
  lastPositive + ANALYSIS_DURATION > millis();
}

int powerLevel = 0;
bool isPowerDangerous()
{
  return powerLevel > DANGER_THRESHOLD;
}
long lastUpdateDisplayPowerLevel = millis();

bool powerSetupTime = true;

int currentSoundPitch=300;
long triggeredLastSound = millis();

bool high = false;
void updateSound()
{
  if(powerLevel > DANGER_THRESHOLD)
  {
    if(millis()%400 < 200)
      tone(SOUND_PIN,300);
    else tone(SOUND_PIN,6000);
  }
  else
  {
  /*if(powerLevel > DANGER_THRESHOLD/2)
  {*/
    if(triggeredLastSound +
    map(powerLevel, 0,DANGER_THRESHOLD, 10000,500)
     < millis())
    {
    tone(SOUND_PIN,map(powerLevel, 0, DANGER_THRESHOLD, 5000,1000));
    triggeredLastSound = millis();
    }
    else if(triggeredLastSound + 200 < millis())
     tone(SOUND_PIN,16000);
  }  
}

long lastTimeWarningForDanger = millis();
void loopPower()
{
  updateSound();
    
  if(millis() < lastUpdatePowerLevel +40) return;
  
  int currentPowerLevel= map(analogRead(potIn),0,1024,0,255); 

  if(abs(currentPowerLevel - powerLevel) > 20 || powerSetupTime)
  {
    powerLevel = currentPowerLevel;
    powerSetupTime = false;
    showPower(powerLevel, isPowerDangerous());
    lastUpdatePowerLevel = millis();
    analogWrite(9, 0);
    if(isPowerDangerous())
    {
    analogWrite(10, 0);
    analogWrite(RED_LED_PIN, 0);
    analogWrite(VIBRATOR_PIN, 0);
    }
    else{
    analogWrite(10, map(DANGER_THRESHOLD-powerLevel, 0,DANGER_THRESHOLD,0,100));

    if(powerLevel > DANGER_THRESHOLD*0.8)
        analogWrite(RED_LED_PIN, 255);
    
    analogWrite(VIBRATOR_PIN, map(powerLevel, 0,DANGER_THRESHOLD,0,(MAX_VIBRATION_PASSIVE_MOD*8)/10));
    
    }
    return;
  }

  if(isPowerDangerous())
  {
    if(millis() > lastTimeWarningForDanger + 700)
    {
      showDanger();
      analogWrite(9, 50);
      analogWrite(10, 100);
      analogWrite(RED_LED_PIN, 255);
      analogWrite(VIBRATOR_PIN, MAX_VIBRATION_PASSIVE_MOD);
      lastTimeWarningForDanger = millis();
      dangerMessageIsBeingDisplayed = true;
    }
    if(millis() > lastTimeWarningForDanger + 400)
    {
      analogWrite(9, 0);
      analogWrite(10, 0);
      analogWrite(RED_LED_PIN, 0);
      int maxVib = MAX_VIBRATION_PASSIVE_MOD;
      analogWrite(VIBRATOR_PIN, maxVib*8/10);
      dangerMessageIsBeingDisplayed = false;
    }
  }
  
}


int getVibrationLevelGivenPower()
{
  if(powerLevel < DANGER_THRESHOLD)
  return map(powerLevel,(int)0, DANGER_THRESHOLD,0 ,MAX_VIBRATION_PASSIVE_MOD);
  else return MAX_VIBRATION_PASSIVE_MOD;
}

void loopButton(int powerLevel)
{
  if( millis() < lastUpdatePowerLevel + 2000)
    return;
  if(isAnalyzing())
  {
    if(!dangerMessageIsBeingDisplayed)
      showAnalyzing();
  }
  else if(!dangerMessageIsBeingDisplayed)
    showResult();
  
  int sensorVal = digitalRead(buttonPin);
  if (sensorVal == HIGH)
  {
    lastPositive = millis();
    hasTriggerButtonBeenFired = true;
  }

    if(isTriggered())
    {
       showIsCapturing();
       currentAnalyzis = String(random());
      Serial.println("triggerded!");

      long start = millis();

       tone(SOUND_PIN,5000);

       int maxPowerReached = map(
        getVibrationLevelGivenPower(),0,MAX_VIBRATION_PASSIVE_MOD,
        50,255);
       int maxPowerReachedDuringBootup = maxPowerReached*0.6;

       analogWrite(VIBRATOR_PIN, maxPowerReached*0.8 );
       delay(40);

       //ramping up
      for(int i = 0 ; i < maxPowerReached ; i++)
      {
              analogWrite(VIBRATOR_PIN, i*0.8);
              analogWrite(lightPin, random(i*0.6));
              delay(40);

      }

      tone(SOUND_PIN,16000);
    digitalWrite(LED_BUILTIN, HIGH);

  analogWrite(lightPin, 0);
           analogWrite(VIBRATOR_PIN, 0);
    delay(200);

    int nbFlashes = powerLevel / 50 +1;
      for(int i = 0 ; i < nbFlashes ; i++)
      {
        analogWrite(lightPin, 0);
           analogWrite(VIBRATOR_PIN, 0);
            delay(100+random(300));
           analogWrite(lightPin, maxPowerReached);
           analogWrite(VIBRATOR_PIN, maxPowerReached);
           delay(100+random(500));
           
      }

       analogWrite(lightPin, 0);
           analogWrite(VIBRATOR_PIN, 0);
    delay(200);

      for(int i = maxPowerReached ; i >= 0 ; i--)
      {
        if(i >= getVibrationLevelGivenPower()*0.8)
              analogWrite(VIBRATOR_PIN, i);
              
        analogWrite(lightPin, random(i*0.8*powerLevel/255));
        delay(20);
      }
  } else {
     digitalWrite(lightPin, LOW);
  }
}

void loop() {
  loopPower();
  loopButton(powerLevel);

    if(millis() < 1000 + bootupTime)
    {
  display.clearDisplay();
       display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextSize(2);
  display.println("Booting up");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.display();
    }
}

