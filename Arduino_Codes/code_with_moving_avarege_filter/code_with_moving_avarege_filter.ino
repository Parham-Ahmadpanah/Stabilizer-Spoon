
#include <Wire.h> 
#include <Servo.h> 
#include <SimpleKalmanFilter.h>

Servo myservoX;  
Servo myservoY; 

long accelX, accelY, accelZ; 
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
float gyroX_cal, gyroY_cal, gyroZ_cal;
float angle_pitch, angle_roll;
float angle_roll_acc, angle_pitch_acc;
float angle_pitch_output, angle_roll_output;
int acc_calibration_value = 1000;                
float angle_acc;

long loop_timer;
int servoXpos=0;
int servoYpos=80;
int count = 0;

//moving average filter
#define WINDOW_SIZE 20
int INDEX1 = 0;
int VALUE1 = 0;
int SUM1 = 0;
int READINGS1[WINDOW_SIZE];
int Averaged1 = 0;

int INDEX2 = 0;
int VALUE2 = 0;
int SUM2 = 0;
int READINGS2[WINDOW_SIZE];
int Averaged2 = 0;

int INDEX3 = 0;
int VALUE3 = 0;
int SUM3 = 0;
int READINGS3[WINDOW_SIZE];
int Averaged3 = 0;

void setup(){
  Serial.begin(9600);
  
  //myservoX.attach(9);   // pitch  
  //myservoY.attach(10);  // roll
  //myservoX.write(40);  
  //myservoY.write(80);
  Wire.begin(); 
  setupMPU();       // mpu6050 اتصال
  pinMode(13,OUTPUT);
  Serial.println("caliberating MPU6050");  

  for(int i=0; i<2000; i++){
    if(i %125 == 0){Serial.print("."); }
    recordGyroRegisters();
    gyroX_cal += gyroX;
    gyroY_cal += gyroY;
    gyroZ_cal += gyroZ;
    delayMicroseconds(3700);
  }
  gyroX_cal /= 2000;
  gyroY_cal /= 2000;
  gyroZ_cal /= 2000;
  Serial.print("gyroX_cal: ");
  Serial.print(gyroX_cal);
  Serial.print("  gyroY_cal: ");
  Serial.print(gyroY_cal);
  Serial.print("  gyroZ_cal: ");
  Serial.print(gyroZ_cal);
  //delay(2000);
  loop_timer = micros();
}

void loop(){
  recordAccelRegisters();
  recordGyroRegisters();
  
  gyroX -= gyroX_cal;
  gyroY -= gyroY_cal;
  gyroZ -= gyroZ_cal;
 
  SUM1 = SUM1 - READINGS1[INDEX1];       // Remove the oldest entry from the sum
  VALUE1 = gyroX;        // Read the next sensor value
  READINGS1[INDEX1] = VALUE1;           // Add the newest reading to the window
  SUM1 = SUM1 + VALUE1;                 // Add the newest reading to the sum
  INDEX1 = (INDEX1+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size
  Averaged1 = SUM1 / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
  gyroX=Averaged1;

  SUM2 = SUM2 - READINGS2[INDEX2];  
  VALUE2 = gyroY;      
  READINGS2[INDEX2] = VALUE2;           
  SUM2 = SUM2 + VALUE2;              
  INDEX2 = (INDEX2+1) % WINDOW_SIZE; 
  Averaged2 = SUM2 / WINDOW_SIZE;
  gyroY=Averaged2;

  SUM3 = SUM3 - READINGS3[INDEX3];  
  VALUE3 = gyroZ;      
  READINGS3[INDEX3] = VALUE3;           
  SUM3 = SUM3 + VALUE3;              
  INDEX3 = (INDEX3+1) % WINDOW_SIZE; 
  Averaged3 = SUM3 / WINDOW_SIZE;
  gyroZ=Averaged3;
  
/*
Serial.print(gyroX);
Serial.print(",");
Serial.println(Averaged);
*/

  angle_roll += gyroX * 0.000122;
  angle_pitch += gyroY * 0.000122;   

  angle_roll += angle_pitch * sin(gyroZ * 0.000002131);             
  angle_pitch -= angle_roll * sin(gyroZ * 0.000002131);   
//
  servoXpos = map(angle_pitch, 90.00,-90.00,0,180);
  servoYpos = map(-angle_roll, -90.00,90.00,0,180);


   count++;
   
   while(micros() - loop_timer < 8000);{
    if(count==1){
    if(servoYpos >=0 && servoYpos <=180){
         myservoY.write(servoYpos-10);

        }
     }
    if(count==2){
      count=0;
      if(servoXpos >=0 && servoXpos <=180){
         myservoX.write(servoXpos-50 );

      }
     }
   }
  //
  loop_timer += 8000;
  }
  
void setupMPU(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
 /* Wire.beginTransmission(0b1101000);
  Wire.write(0x1A);
  Wire.write(0x03);
  Wire.endTransmission(); */
}

void recordAccelRegisters(){
  Wire.beginTransmission(0b1101000);  
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  while(Wire.available() < 6);
  accelX = Wire.read() << 8 | Wire.read();
  accelY = Wire.read() << 8 | Wire.read();
  accelZ = Wire.read() << 8 | Wire.read();
}

void recordGyroRegisters(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();
}
