/*
===============
**************  https://github.com/vinojv96/  *********
===============
*/

#define DELAY 200 // time to wait after a clap (prevents detecting lots of claps as 1 signal)
#define RELAY_PIN 13
#define SENSOR_PIN 3
#define DIFF_MAX 600 // max milliseconds difference between 2 claps


void setup() 
{
  pinMode(SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
}

boolean firstClap = false;
boolean secondClap = false;
boolean offClap1 = false;

unsigned long firstClapTime;
unsigned long offClap1Time;
unsigned long lastOperation = 0;

void loop()
{
  if(millis() - lastOperation < DELAY) // wait DELAY time because sound lasts some time
      return;

  if(digitalRead(SENSOR_PIN) == HIGH) // someone's clapping
  {
      if(firstClap == false)
      {
          firstClap = true;
          firstClapTime = millis();
      }
      else if(secondClap == false)
      {
          if(millis() - firstClapTime < DIFF_MAX) // make sure two second clap were made in short period of time
          {
              secondClap = true;
          }
      }
      else // fistClap and secondClap = true, so we are now turning it off
      {
          if(offClap1 == false)
          {
              offClap1 = true;
              offClap1Time = millis();
          }
          else // first OFF clap was made, now is the second
          {
              unsigned long offClap2Time = millis();
              if(offClap2Time - offClap1Time < DIFF_MAX) // make sure two second clap were made in short period of time
              {
                  firstClap = false;
                  secondClap = false;
              }
              offClap1 = false;     
          }
      }

      lastOperation = millis();
  }

  unsigned long diff = millis() - firstClapTime;
  if(firstClap == true && secondClap == false && diff > DIFF_MAX) // reset firstClap value if there was no second clap
  {
      firstClap = false;
  }

  diff = millis() - offClap1Time;
  if(offClap1 == true && diff > DIFF_MAX) // reset offClap value if there was no second clap in time
  {
      offClap1 = false;
  }

  digitalWrite(RELAY_PIN, secondClap); // secondClap is always true if ON and false if OFF
}

