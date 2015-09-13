#include <ATX2.h>
#include "ATX2_enc.h"

void setup()
{
  Serial.begin(9600);
  encoder();
}

void loop()
{
  OK();
  FD(15);
  while(encCnt<40)Serial.println(encCnt);;
  encCnt=0;
  AO();
  OK();
  BK(15);
  while(encCnt<40)Serial.println(encCnt);;
  encCnt=0;
  AO();
}
//void tong() {
//   
//}
//void tong2(int x,int z,char y) {
//  
//while(1){
//  if(y=='F'&&i<z){
//    FD(x);
//  }else if(y=='R'&&i<z){
//    SL(-z);
//    SR(z);
//  }else{
//    AO();i=0;goto tong;
//  }
//  
//}
//tong:
//;
//}
