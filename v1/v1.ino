#include <ATX2.h>

void setup()
{
  Serial.begin(9600); 
  servo(1,58);
  delay(100);
  servo(2,85);
  delay(100);
 
}

void loop()
{
   Serial.println(analogRead(A6));
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
