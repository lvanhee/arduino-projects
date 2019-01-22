const uint64_t receiverAddress = 0x0000000000000001LL;
//to improve with multiple pipes. See: https://maniacbug.github.io/RF24/starping_8pde-example.html

bool isSending=false;
void setupRadio()
{
  radio.begin();
  radio.setRetries(15,15);
  radio.openWritingPipe(receiverAddress);
  
  radio.startListening();
}

long timeForNextCall = 0;

void loopRadio()
{
    if(!isSending && timeForNextSend())
      {
        radio.stopListening();
        Serial.println("time for next send!");
        timeForNextCall = millis() + 1000;
        sendInfo();
        isSending = true;
        return;
      }
      
    if(isSending)
    {
      if(!isSentFinished()) return;
      else
      {
        Serial.println("send finished!");
        isSending = false; 
        radio.startListening();
        return;
      }
      }
    
   //  Serial.println("updateInfo");
    updateInfo();
    
    
}

bool isSentFinished()
{
  bool sent,failed,incoming;
  radio.whatHappened(sent,failed,incoming);
  return (sent || failed);	
}

bool timeForNextSend()
{
  return (millis() > timeForNextCall);
}

void sendInfo()
{
    
    radio.startWrite( &scores, sizeof(scores) );
    
    Serial.print("sending data:");
    for(int i = 0 ; i < 10 ; i ++)
    {
      Serial.print(scores[i]);
      Serial.print(",");
    }
    
    Serial.println();
    
}

void updateInfo()
{
  while(radio.available())
  {
    int data[10];
    radio.read( &data, 10*sizeof(unsigned int) );
    
   Serial.print("incoming data:");
    for(int i = 0 ; i < 10 ; i ++)
    {
      Serial.print(data[i]);
      Serial.print(",");
    }
    
    Serial.println();
    
    
    updateScore(data);
  }
}
