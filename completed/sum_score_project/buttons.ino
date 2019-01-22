
const int plusButton = 5;
const int minusButton = 6;

unsigned long startPressOfPlusButton=4294967295;
bool isHighIncreased=true;

unsigned long startPressOfMinusButton =4294967295;
bool hasDecreasedSinceMinusIsPressed = true;

void setupButtons()
{
    pinMode(minusButton, INPUT_PULLUP);
    startPressOfPlusButton = millis() + 10000;
    startPressOfMinusButton = millis() + 10000;
    
    pinMode(plusButton, INPUT_PULLUP);
}


void loopButtons()
{
  if (digitalRead(plusButton) == HIGH)
  {
    if(startPressOfPlusButton + 50 <= millis())
      if(!isHighIncreased)
       { 
          increaseScore();
          isHighIncreased = true;
       }
  }
  else 
  {
  isHighIncreased = false; 
  startPressOfPlusButton = millis();
  }
  
    
  if (digitalRead(minusButton) == HIGH)
  {
    if(startPressOfMinusButton + 50 <= millis())
      if(!hasDecreasedSinceMinusIsPressed)
       { 
          decreaseScore();
          hasDecreasedSinceMinusIsPressed = true;
       }
  }
  else 
  {
  hasDecreasedSinceMinusIsPressed = false; 
  startPressOfMinusButton = millis();
  }
}
