#include <ZeroTime.h>

void setup()
{
  Serial.begin(9600);

  // set time to 1pm Mar. 2, 2016
  setTime(13, 0, 0, 2, 3, 2016);
}

void loop()
{
  // Print date...
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(year());
  Serial.print("\t");
  
  // ...and time
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
 
  Serial.println();
  
  delay(1000);
}
