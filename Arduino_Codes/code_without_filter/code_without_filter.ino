
#include <Wire.h> 
#include <Servo.h> 
Servo myservoX;  
Servo myservoY; 

long accelX, accelY, accelZ; 
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;//final gyro we get
float rotX, rotY, rotZ;//dont need them
float gyroX_cal, gyroY_cal, gyroZ_cal;//GYROSCOPE data from sesnor for each loop 
float angle_pitch, angle_roll;//two angkes which we want to control
float angle_roll_acc, angle_pitch_acc;//acceleration of these 2 angles
float angle_pitch_output, angle_roll_output;
int acc_calibration_value = 1000;          //for calibrating mpu6050
float angle_acc;

long loop_timer;
int servoXpos=0;
int servoYpos=80;
int count = 0;

void setup(){
  Serial.begin(9600);
  
  myservoX.attach(9);   // pitch  ,servomotor controlls
  myservoY.attach(10);  // roll   ,servomotor controlls
  myservoX.write(40);  
  myservoY.write(80);
  Wire.begin(); 
  setupMPU();       // mpu6050 اتصال
  //delay(1000);
  Serial.println("caliberating MPU6050");  
//calibrating mpu6050
  for(int i=0; i<2000; i++){
    if(i %125 == 0){Serial.print("."); }
    recordGyroRegisters();
    gyroX_cal += gyroX;
    gyroY_cal += gyroY;
    gyroZ_cal += gyroZ;
    delayMicroseconds(3700);
  }
  gyroX_cal /= 2000;//lower the gyro data by dividing it by 20000
  gyroY_cal /= 2000;
  gyroZ_cal /= 2000;
  Serial.print("gyroX_cal: ");
  Serial.print(gyroX_cal);
  Serial.print("  gyroY_cal: ");
  Serial.print(gyroY_cal);
  Serial.print("  gyroZ_cal: ");
  Serial.print(gyroZ_cal);
  //delay(2000);
  loop_timer = micros();// micros give us period between start time until it runs
}

void loop(){
  recordAccelRegisters();//take 3 acceleration data from sensor
  recordGyroRegisters();//take 3 gyro data from sensor
  
  gyroX -= gyroX_cal;//putting in cal
  gyroY -= gyroY_cal;
  gyroZ -= gyroZ_cal;
 //calculating angles to control servomotors
  angle_roll += gyroX * 0.000122;
  angle_pitch += gyroY * 0.000122;   


  angle_roll += angle_pitch * sin(gyroZ * 0.000002131);             
  angle_pitch -= angle_roll * sin(gyroZ * 0.000002131);   

//guiding servo motors according to angles
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
  loop_timer += 8000;// it controls that next loop servos come to the first place or not
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
