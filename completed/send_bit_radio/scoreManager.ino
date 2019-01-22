
int sumScores()
{
  int total = 0;
  for(int i = 0 ; i < 5 ; i ++)
  {
    total += scores[2*i];
    total -= scores[2*i+1];
  }
  return total;
}

void increaseScore()
{
  scores[2*getCurrentPlayer()]++;
  Serial.print("Score increased, new score:");
  printScore();
}

void decreaseScore()
{
  scores[2*getCurrentPlayer()+1]++;
    Serial.print("Score decreased, new score:");
  printScore();
}

void printScore()
{
  for(int i = 0 ; i < 10 ; i ++)
  {
    Serial.print(scores[i]);
    Serial.print(",");
  }
  Serial.print("total:");
  Serial.print(sumScores()); 
  Serial.println();
   
}

int getCurrentPlayer()
{
  return myID;
}

void updateScore(int* data)
{
  bool changed = false;
 for(int i = 0 ; i < 10 ; i ++)
 {
   if(data[i] > scores[i]) {changed = true; scores[i] = data[i];}
 }
 
 if(changed) {Serial.println("Updated!"); printScore();}
}
