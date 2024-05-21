
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

SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.001);
// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;

//PID
const int numOfReadings = 10; 
#define WINDOW_SIZE 10 
int INDEX = 0;
int VALUE = 0;
int READINGS[WINDOW_SIZE];
float total = 0; //majmoo araye ha
// PID Parameters
float Actual = 0; //miangin fasele mohasebe shde
float Setpoint=0; //fasele mabna
float Error=0; //khataye mohasebe shode
float Integral=0; //khataye antegrali
float Derivative=0; //khataye moshtaghg
float Drive=0; //meghdar PID mohasebe shode
float Previous_error=0; //khateye ghabli
//Coefficients PID
float kp=90;
float ki=2;
float kd=1;
float dt=0.01; //baze zamani moshtagh giri




void setup(){
  Serial.begin(9600);
  
  //myservoX.attach(9);   // pitch  
  //myservoY.attach(10);  // roll
  //myservoX.write(40);  
  //myservoY.write(80);
  Wire.begin(); 
  setupMPU();       // mpu6050 اتصال
  //delay(1000);
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
/*
  float estimated_X = simpleKalmanFilter.updateEstimate(gyroX);
  float estimated_Y = simpleKalmanFilter.updateEstimate(gyroY);
  float estimated_Z = simpleKalmanFilter.updateEstimate(gyroZ);
  */
  Serial.print(gyroX);
  Serial.print(",");
//PID
  total=total-READINGS[INDEX];     
  VALUE= gyroX;        
  READINGS[INDEX] = VALUE;         
  total = total + VALUE;                 
  INDEX = (INDEX+1) % WINDOW_SIZE;
  
  Actual=total/numOfReadings; // mohasebe fasele miangin
  Error=Setpoint-Actual;
  Integral=Integral+(Error*dt);
  Derivative=(Error-Previous_error)/dt;
  gyroX=(Error*kp)+(Integral*ki)+(Derivative*kd);
  Previous_error=Error;
  total=0;


  Serial.println(gyroX);
  //
  angle_roll += gyroX * 0.000122;
  angle_pitch += gyroY * 0.000122;  

  angle_roll += angle_pitch * sin(gyroZ * 0.000002131);             
  angle_pitch -= angle_roll * sin(gyroZ * 0.000002131);   

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
