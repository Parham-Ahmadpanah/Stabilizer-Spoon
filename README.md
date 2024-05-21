# Stabilizer-Spoon
## Description
Utilizing stabilizing mechanisms in aerial platforms for motion control. This project aims to develop a stabilizing mechanism to mitigate unwanted movements and vibrations. The primary focus is on utilizing a microcontroller and servo motor to achieve this goal. The project’s outcome is expected to contribute to the development of more affordable and efficient stabilizing mechanisms for various industries.

![Self-Balancing Robot](Data/Stabilizer_Spoon.gif)
<video width="320" height="240" controls>
  <source src="Data/Stabilizer_Spoon.gif" type="video/gif">
  Your browser does not support the video tag.
</video>

## Objectives
The main objective of this project is to create a stabilizing mechanism that can reduce undesired movements and vibrations. By utilizing a microcontroller and servo motor, the aim is to build a prototype that can effectively stabilize motion. The project’s results could lead to further research and development of improved and cost-effective stabilizing solutions.

## Components Used
2x Servo Motor SG90
Gyroscope Module MPU6050
Jumper Wires
Arduino UNO R3
3D-printed Plastic Components
The project utilizes a servo motor 90SG for motion control. The servo motor provides precise control over angular position, velocity, and acceleration, making it suitable for applications requiring high efficiency and accuracy.

## Servo Motor 90SG
The motor we are using is the 90SG servo motor, which is a standard electric motor equipped with servo control mechanism. This motor is a rotary actuator that allows precise control over angular or linear position.
Description
The SG90 servo motor has three pins: S, Vcc, and GND. The S pin is the signal pin that connects to the PWM pin of the Arduino. The +ve and -ve pins connect to 5 volts and GND on the Arduino, respectively. The wiring scheme for the servo motor is provided in Figure 2.
<div style="display: flex;">
    <img src="Data\Picture1.png" alt="Image 1" style="width: 50%;">
    <img src="Data\Picture2.png" alt="Image 2" style="width: 50%;">
</div>

The connection of the motor to the Arduino is illustrated in Figure 3.
<!-- <div style="display: flex;">
    <img src="Data\Picture1.png" alt="Image 1" style="width: 50%;">
    <img src="Data\Picture2.png" alt="Image 2" style="width: 50%;">
</div> -->
Specifications of Motor 90SG:
Operating Voltage: 4.8 to 6 volts (typically 5 volts)
Stall Torque: 1.8 kg/cm (at 4.8 volts)
Maximum Stall Torque: 2.2 kg/cm (at 6 volts)
Operating Speed: 0.1 sec/60 degrees (at 4.8 volts)
Gear Type: Metal
Rotation: 180-0 degrees
Motor Weight: 13.4 grams

These specifications provide a comprehensive overview of the 90SG servo motor, including its voltage range, torque capabilities, speed, gear type, rotation angle, and weight. For further details, please refer to the motor’s documentation.
