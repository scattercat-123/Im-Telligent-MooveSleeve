#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "EMGFilters.h"
#define TIMING_DEBUG 1
#define SensorInputPin A0

int sensorValue=0;
int gsr_average=0;
const int GSR=A1;

EMGFilters myFilter;
int sampleRate = SAMPLE_FREQ_1000HZ;
int humFreq = NOTCH_FREQ_50HZ;

unsigned long timeStamp;
unsigned long timeBudget;

void setup(){
  Serial.begin(9600);
      myFilter.init(sampleRate, humFreq, true, true, true);

          timeBudget = 1e6 / sampleRate;
}

void loop(){
  long sum=0;
    timeStamp = micros();
    int Value = analogRead(SensorInputPin);
    int DataAfterFilter = myFilter.update(Value);
    timeStamp = micros() - timeStamp;

  for(int i=0;i<10;i++)           //Average the 10 measurements to remove the glitch
      {
      sensorValue=analogRead(GSR);
      sum += sensorValue;
      delay(5);
      }
   gsr_average = sum/10;

Serial.print(abs(DataAfterFilter - 3)); 
  Serial.print(",");

      Serial.println(gsr_average);

   
  delay(90);}
    
