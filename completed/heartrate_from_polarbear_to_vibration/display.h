long lastDisplayUpdate = millis();
long timeLastNewBeat = millis();

#define WARN_LEVEL 70

void setupDisplay()
{
  pinMode(19, OUTPUT);
  digitalWrite(19, LOW);

  pinMode(18, OUTPUT);

}

bool isTimeForNewHeartBeat(int heartRate)
{
  if (heartRate <= 0) return isTimeForNewHeartBeat(60);
  long timeSinceLastBeat = millis() - timeLastNewBeat;
  return timeSinceLastBeat > ((long)60000) / heartRate;
}

float avgHR=90;

int count=90;

boolean wasTooHigh=false;
void loopDisplay(int heartRate)
{
  if (heartRate < 0) return;
  if (millis() < lastDisplayUpdate + 50)
    return;
  lastDisplayUpdate = millis();

  if(avgHR > heartRate)
    avgHR=avgHR*0.999+heartRate*0.001;

    

    if(avgHR < WARN_LEVEL){
      if(wasTooHigh)
      {
        for(int i = 0 ; i <5 ; i ++)
        {
           digitalWrite(19, HIGH);
            delay(100);
          digitalWrite(19, LOW);
          delay(100);
        }
        wasTooHigh=false;
      }
     
    }

  if(avgHR > WARN_LEVEL) wasTooHigh=true;

  
  if (isTimeForNewHeartBeat(heartRate))
  {
     /*   Serial.print("was too high:");
    Serial.println(wasTooHigh);*/
    
     boolean shouldDisplayHeartRate= heartRate > WARN_LEVEL;
     //(heartRate-avgHR) > 10 || (avgHR > WARN_LEVEL && count >=97);
     Serial.print(avgHR);
     Serial.print(" ");
     Serial.print(heartRate);
     Serial.println();
     count++;
     count %=100;
  //  Serial.println("count:"+String(count));
    timeLastNewBeat = millis();
    if(shouldDisplayHeartRate)
    {
       digitalWrite(19, HIGH);
   //    digitalWrite(18, HIGH);
       delay(200);
       digitalWrite(19, LOW);
     // digitalWrite(18, LOW);
    // avgHR=avgHR*0.99+heartRate*0.01;
    }
  }
}
