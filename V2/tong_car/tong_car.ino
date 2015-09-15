/* kkao start 13/11/15 */

  
#define red_color   1   
#define green_color 2   
#define yellow_color 3   
#define none_color  4

#define sampling_rate  50   

// Color sensor pin 
#define S0  13                               
#define S1  3                               
#define S2  15                 
#define S3  14                             
#define taosOutPin  2                                      

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

// Encoder 
#define pluse2cm 1

#define RAD_TO_DEG 57.27272727272727f

#include <TimerOne.h>
#include <ATX2.h>
#include "ATX2_enc.h"
#include <Wire.h>
#include <HMC5883L.h>


HMC5883L compass;
float INput_a[6]={0};
byte Multi_line = 0x00;
float Object = 0;
unsigned long time = 0;

float distance = 0;

float OB_distance = 0;

float heading_set = 0;
float heading = 0;

float error_LR;
uint8_t IR_state;
uint8_t color_can;

int i;
void setup()
{
	Serial.begin(115200);
	encoder();
        Wire.begin();
	compass = HMC5883L(); //new instance of HMC5883L library
  	setupHMC5883L(); //setup the HMC5883L

	//this will put in 1:1, highest sensitivity of color sensor
    digitalWrite(S0, HIGH); //S0
    digitalWrite(S1, HIGH); //S1


    	Serial.println("start_cali ? ");
	OK();
    	Serial.println("start_cali begin");
	int index = 200;
	while (index > 0)
	{
		index -- ;
		heading_set = Smooth_filter( getHeading(), heading_set);
		delay(20);
	}
		Serial.println(heading_set);
	 	Serial.println("start_cali end");

	OK();
    	Serial.println("start");

	// Timer1.initialize(20000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
	// Timer1.attachInterrupt( Sampling ); // attach the service routine here
    	time = millis();
}

void loop()
{
	unsigned long time_tmp = millis();
	if (time_tmp - time >= 20)
	{
		time = time_tmp;
		Sampling();
	}
	// delay(1000);

}

void Sampling()
{

	heading = getHeading() - heading_set;

	// Update_encoder();
	// // distance

	// Update_IR(); 	
	// // error_LR 
	// // IR_state 

	// color_can = colorRead(); // 0 not ready, 1 red, 2 yellow, 3 green, 4 none

	// OB_distance = Update_object_distance();

	// Serial.print(INput_a[0]); Serial.print("---");
	// Serial.print(INput_a[1]); Serial.print("---");
	// Serial.print(INput_a[2]); Serial.print("---");
	// Serial.print(INput_a[3]); Serial.print("---");
	// Serial.print(INput_a[4]); Serial.print("---");
	Serial.println(heading);


	// Motor_drive(0, 0);
	// ARM_Move(); // 0 Down, 1 UP
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

void Update_encoder()
{
	distance += (float)encCnt*pluse2cm;
	encCnt = 0;
}

void Update_IR()
{
	float error_LR_tmp;
	uint8_t IR_state_tmp;

	INput_a[0] = Smooth_filter( (((float)analogRead(IR1) * 0.001f) - IR1_offset)/IR1_min, INput_a[0]);
	INput_a[1] = Smooth_filter( (((float)analogRead(IR2) * 0.001f) - IR2_offset)/IR2_min, INput_a[1]);
	INput_a[2] = Smooth_filter( (((float)analogRead(IR3) * 0.001f) - IR3_offset)/IR3_min, INput_a[2]);
	INput_a[3] = Smooth_filter( (((float)analogRead(IR4) * 0.001f) - IR4_offset)/IR4_min, INput_a[3]);
	INput_a[4] = Smooth_filter( (((float)analogRead(IR5) * 0.001f) - IR5_offset)/IR5_min, INput_a[4]);
	INput_a[5] = Smooth_filter( (((float)analogRead(IR6) * 0.001f) - IR6_offset)/IR6_min, INput_a[4]);

	error_LR_tmp =  3.0f*dead_band (0.3f, INput_a[0]) + 2.0f*dead_band (0.3f, INput_a[0]) + dead_band (0.3f, INput_a[0]) - dead_band (0.3f, INput_a[0]) - 2.0f*dead_band (0.3f, INput_a[0]) - 3.0f*dead_band (0.3f, INput_a[0]);

	for (byte x = 0 ; x < 6 ; x++)
	{
		if (INput_a[0] > 0.5f)
		{
			IR_state_tmp |= 1;
		}else{
			IR_state_tmp |= 0;
		}

		IR_state_tmp<<1;
	}

	/* update data here*/
	error_LR = error_LR_tmp;
	IR_state = IR_state_tmp;

}

int colorRead()// 0 not ready, 1 red, 2 yellow, 3 green, 4 none
{/*
	none      w:23 r:80 g:77 b:47
	yellow    w:2 r:5 g:6 b:10
	red       w:5 r:7 g:19 b:13
	green     w:5 r:21 g:15 b:13

*/
 
	static float white_tmp, red_tmp, blue_tmp, green_tmp;
	static byte step;
	static byte color; // 0 not ready, 1 red, 2 yellow, 3 green, 4 none
    // digitalWrite(LED, HIGH);

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
   		white_tmp = readPulse;
    }

    if(step == 1)
    {//red
   		red_tmp = readPulse;
    }

    if(step == 2)
    {//blue
   		blue_tmp = readPulse;
    }

    if(step == 3)
    {//green
   		green_tmp = readPulse;
    }
    //power OFF mode-  LED off and both channels "low" 
    // digitalWrite(LED, LOW);
    digitalWrite(S0, LOW); //S0
    digitalWrite(S1, LOW); //S1
     
    step++;

    if (step == 4)
    {
    	byte temp = 0; // sent data out 
    	step = 0;		// reset step

    	if (white_tmp > 10) 
    	{
    		return none_color;
    	}else if (red_tmp > 13)
    	{
    		return green_color;
    	}else if(green_tmp > 13)
    	{
    		return red_color;
    	}else{
    		return yellow_color;
    	}
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
  error = compass.SetScale(0.88); //Set the scale of the compass.
  if(error != 0) Serial.println(compass.GetErrorText(error)); //check if there is an error, and print if so

  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) Serial.println(compass.GetErrorText(error)); //check if there is an error, and print if so
}

float getHeading()
{
	static float heading;
	//Get the reading from the HMC5883L and calculate the heading
	MagnetometerScaled scaled = compass.ReadScaledAxis(); //scaled values from compass.
	heading = Smooth_filter(atan2(scaled.YAxis, scaled.XAxis), heading);

	// Correct for when signs are reversed.
	//  if(heading < 0) heading += 2*PI;
	//  if(heading > 2*PI) heading -= 2*PI;

	return heading * RAD_TO_DEG; //radians to degrees
}

float dead_band (float rank, float data)
{
	if ((data >= rank)||(data <= -rank))
	{
		return data;
	}else{
		return 0;		
	}
}
