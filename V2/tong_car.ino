/* kkao start 13/11/15 */

#define test  13       

#define sampling_rate  50   

// Color sensor pin 
#define S0  13                               
#define S1  14                               
#define S2  15                 
#define S3  3                             
#define taosOutPin  2                     
#define LED  13                           

//   						front	
// IR line sensor pin  L 1 2 3 4 5 6 R
#define IR1  A0                               
#define IR2  A1                               
#define IR3  A2                 
#define IR4  A3                              
#define IR5  A4                     
#define IR6  A5    

#define IR1_min  -0.80f                               
#define IR2_min  -0.51f                            
#define IR3_min  -0.72f              
#define IR4_min  -0.72f                           
#define IR5_min  -0.65f                  
#define IR6_min  -0.72f 

#define IR1_offset  0.98f                               
#define IR2_offset  0.72f                            
#define IR3_offset  0.90f              
#define IR4_offset  0.87f                           
#define IR5_offset  0.79f                  
#define IR6_offset  0.86f 

// IR object sensor pin  L 1 2 3 4 5 R
#define IR7  A6                   

// Servos pin
#define servo_arm 7                               
#define servo_hand 8        



#include <TimerOne.h>
#include <ATX2.h>
#include "ATX2_enc.h"
#include <Wire.h>
#include <HMC5883L.h>

HMC5883L compass;
float INput_a[6]={0};
byte Multi_line = 0x00;
float Object = 0;
unsigned long i=0;



void setup()
{
	Serial.begin(115200);
	encoder();

	compass = HMC5883L(); //new instance of HMC5883L library
  	setupHMC5883L(); //setup the HMC5883L

	//this will put in 1:1, highest sensitivity of color sensor
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, HIGH); //S1
    pinMode(test, OUTPUT);






	OK();// Wait for OK button
	Timer1.initialize(1000000/sampling_rate); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	Timer1.attachInterrupt( Sampling ); // attach the service routine here
}

void loop()
{

}

void Sampling()
{
	digitalWrite(test, 1);
	Update_IR();
	Serial.print(INput_a[0]); Serial.print("---");
	Serial.print(INput_a[1]); Serial.print("---");
	Serial.print(INput_a[2]); Serial.print("---");
	Serial.print(INput_a[3]); Serial.print("---");
	Serial.print(INput_a[4]); Serial.print("---");
	Serial.print(INput_a[5]); Serial.println("---");
	digitalWrite(test, 0);
}

float Update_mag()
{
	static float heading;
	heading = Smooth_filter(getHeading(), heading);
	return heading;
}

float Update_object_distance()
{
	static float Object;
	Object = Smooth_filter((float)analogRead(IR7)*0.001f, Object);
	return Object;
}

void Motor_drive(float FW, float LR)
{




	fd2(0, 0);
    AO();
}

void ARM_Move() // 0 Down, 1 UP
{
	if(i==1)
	{
		servo(7,65);
		delay(300);
		servo(8,85);
		delay(200);
	}
	if(i==0)
	{
		servo(8,85);
		delay(200);
		servo(7,58);
		delay(200);
	}
	

	if(i==0)
	{
		servo(7,0);
		delay(200);
		servo(8,180);
		delay(200);
	}
	if(i==1)
	{
		servo(8,180);
		delay(300);
		servo(7,0);
		delay(200);
	}
}

void Update_IR()
{
	static float error_LR;

	INput_a[0] = Smooth_filter( (((float)analogRead(IR1) * 0.001f) - IR1_offset)/IR1_min, INput_a[0]);
	INput_a[1] = Smooth_filter( (((float)analogRead(IR2) * 0.001f) - IR2_offset)/IR2_min, INput_a[1]);
	INput_a[2] = Smooth_filter( (((float)analogRead(IR3) * 0.001f) - IR3_offset)/IR3_min, INput_a[2]);
	INput_a[3] = Smooth_filter( (((float)analogRead(IR4) * 0.001f) - IR4_offset)/IR4_min, INput_a[3]);
	INput_a[4] = Smooth_filter( (((float)analogRead(IR5) * 0.001f) - IR5_offset)/IR5_min, INput_a[4]);
	INput_a[5] = Smooth_filter( (((float)analogRead(IR6) * 0.001f) - IR6_offset)/IR6_min, INput_a[4]);
}

int colorRead()
{ 
	static float white, red, blue, green;
	static byte step;
	static byte color; // 0 none, 1 red, 2 yellow, 3 green
    digitalWrite(LED, HIGH);

   	if(step == 0)
   	{//white
      digitalWrite(S3, LOW); //S3
      digitalWrite(S2, HIGH); //S2
        // Serial.print(" w");
    }

    if(step == 1)
    {//red
      digitalWrite(S3, LOW); //S3
      digitalWrite(S2, LOW); //S2
      // Serial.print(" r");
    }

    if(step == 2)
    {//blue
      digitalWrite(S3, HIGH); //S3
      digitalWrite(S2, LOW); //S2 
      // Serial.print(" b");
    }

    if(step == 3)
    {//green
      digitalWrite(S3, HIGH); //S3
      digitalWrite(S2, HIGH); //S2 
      // Serial.print(" g");
    }

    float readPulse;

    readPulse = pulseIn(taosOutPin, LOW, 80000);

    if(readPulse < .1){
    readPulse = 80000;
    }

   	if(step == 0)
   	{//white
   		white = readPulse;
    }

    if(step == 1)
    {//red
   		red = readPulse;
    }

    if(step == 2)
    {//blue
   		blue = readPulse;
    }

    if(step == 3)
    {//green
   		green = readPulse;
    }
    //power OFF mode-  LED off and both channels "low" 
    digitalWrite(LED, LOW);
    digitalWrite(S0, LOW); //S0
    digitalWrite(S1, LOW); //S1
     
    step++;

    if (step == 4)
    {
    	byte temp = 0;
    	step = 0;

    	return temp;
    }else{
        return 0;
    }
 }

float Smooth_filter(float new_data, float old_data)
{
	float value = old_data + 0.7f*(new_data - old_data);
	return value;
}

void setupHMC5883L(){
  //Setup the HMC5883L, and check for errors
  int error;  
  error = compass.SetScale(1.3); //Set the scale of the compass.
  if(error != 0) Serial.println(compass.GetErrorText(error)); //check if there is an error, and print if so

  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) Serial.println(compass.GetErrorText(error)); //check if there is an error, and print if so
}

float getHeading(){
  //Get the reading from the HMC5883L and calculate the heading
  MagnetometerScaled scaled = compass.ReadScaledAxis(); //scaled values from compass.
  float heading = atan2(scaled.YAxis, scaled.XAxis);

	// Correct for when signs are reversed.
	//  if(heading < 0) heading += 2*PI;
	//  if(heading > 2*PI) heading -= 2*PI;

  return heading * RAD_TO_DEG; //radians to degrees
}