#include <ATX2.h>
#include "ATX2_enc.h"
#include <Wire.h>
#include <HMC5883L.h>
HMC5883L compass;
byte INput_a[6]={0,0,0,0,0,0};
unsigned long i=0;
void setup()
{
  Serial.begin(9600);
  encoder();
   OK();
  servo(7,58);
  delay(100);
  servo(8,85);
  delay(500);

  tong2(50,60,'R');
  tong2(20,40,'F');

}
void loop()
{
  while(1){
    seser_an();
    if(INput_a[6]!=0){
    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==0&&INput_a[3]==0&&INput_a[4]==0&&INput_a[5]==0){
      FD(20);
    }
    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==0&&INput_a[3]==1&&INput_a[4]==0&&INput_a[5]==0){
      SL(-20);
      SR(20);
    }
    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==1&&INput_a[3]==0&&INput_a[4]==0&&INput_a[5]==0){
      SL(20);
      SR(-20);
    }
    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==1&&INput_a[3]==1&&INput_a[4]==1&&INput_a[5]==1){
      AO();

    }
    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==1&&INput_a[3]==1&&INput_a[4]==0&&INput_a[5]==0){
      AO();
    }
    }else{
     AO(); 
     down(0);
     delay(500);
     up(1);
     delay(500);
     tong2(20,52,'F');
     delay(300);
     tong2(40,63,'R');
     delay(100);
     tong2(20,47,'F');
     delay(100);
     down(1);
     up(0);
     AO();
     goto s2;
    }
    
  }
s2:
  while(1){
//    seser_an();
//    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==0&&INput_a[3]==0&&INput_a[4]==0&&INput_a[5]==0){
//      FD(20);
//    }
//    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==0&&INput_a[3]==1&&INput_a[4]==0&&INput_a[5]==0){
//      SL(-20);
//      SR(20);
//    }
//    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==1&&INput_a[3]==0&&INput_a[4]==0&&INput_a[5]==0){
//      SL(20);
//      SR(-20);
//    }
//    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==1&&INput_a[3]==1&&INput_a[4]==1&&INput_a[5]==1){
//      AO();
//      
//    }
//    if(INput_a[0]==0&&INput_a[1]==0&&INput_a[2]==1&&INput_a[3]==1&&INput_a[4]==0&&INput_a[5]==0){
//      AO();
//    }

  }
}
void up(int i){
  if(i==1){
  servo(7,65);
  delay(300);
  servo(8,85);
  delay(200);
  }
  if(i==0){
  servo(8,85);
  delay(200);
  servo(7,58);
  delay(200);
  }
}
void down(int i){
  if(i==0){
    servo(7,0);
    delay(200);
    servo(8,180);
    delay(200);
  }
  if(i==1){
    servo(8,180);
    delay(300);
    servo(7,0);
    delay(200);
  }
  
}
void seser_an(){
  if(analogRead(A0)>640) 
  INput_a[0]=0;
  if(analogRead(A0)<640) 
  INput_a[0]=1;
  if(analogRead(A1)>640) 
  INput_a[1]=0;
  if(analogRead(A1)<640) 
  INput_a[1]=1;
  if(analogRead(A2)>640) 
  INput_a[2]=0;
  if(analogRead(A2)<640) 
  INput_a[2]=1;
  if(analogRead(A3)>640) 
  INput_a[3]=0;
  if(analogRead(A3)<640) 
  INput_a[3]=1;
  if(analogRead(A4)>640) 
  INput_a[4]=0;
  if(analogRead(A4)<640) 
  INput_a[4]=1;
  if(analogRead(A5)>640) 
  INput_a[5]=0;
  if(analogRead(A5)<640) 
  INput_a[5]=1;
  INput_a[6]=5-((analogRead(A6)/100)-1);
}
void tong() {
  i=i+1;
  Serial.println(i); 
  delayMicroseconds(5);  
}
void tong2(int x,int z,char y) {
  encCnt=0;
while(1){
  if(y=='F'&&encCnt<z){
    FD(x);
  }else if(y=='R'&&encCnt<z){
    SL(-x);
    SR(x);
  }else if(y=='B'&&encCnt<z){
    SL(-x);
    SR(x);
  }else{
    AO();encCnt=0;goto tong;
  }
  
}
tong:
;
}
