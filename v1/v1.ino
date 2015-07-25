#include <ATX2.h>

void setup()
{
  servo(1,58);
  delay(100);
  servo(2,85);
  delay(100);
 
}

void loop()
{
   OK();// Wait for OK button
  down(0);
  delay(1000);
  OK();// Wait for OK button
  up(1);
  delay(1000);
  OK();// Wait for OK button
  down(1);
  delay(1000);
  OK();// Wait for OK button
  up(0);
  delay(1000);
}
void up(int i){
  if(i==1){
  servo(1,58);
  delay(200);
  servo(2,85);
  delay(200);
  }
  if(i==0){
  servo(2,85);
  delay(200);
  servo(1,58);
  delay(200);
  }
}
void down(int i){
  if(i==0){
    servo(1,0);
    delay(200);
    servo(2,180);
    delay(200);
  }
  if(i==1){
    servo(2,180);
    delay(300);
    servo(1,0);
    delay(200);
  }
  
}
