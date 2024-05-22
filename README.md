# Stabilizer-Spoon
## Description
Utilizing stabilizing mechanisms in aerial platforms for motion control. This project aims to develop a stabilizing mechanism to mitigate unwanted movements and vibrations. The primary focus is on utilizing a microcontroller and servo motor to achieve this goal. The project’s outcome is expected to contribute to the development of more affordable and efficient stabilizing mechanisms for various industries.

![Self-Balancing Robot](Data/Stabilizer_Spoon.gif)
<video width="320" height="240" controls>
  <source src="Data/Stabilizer_Spoon.gif" type="video/gif">
</video>

## Objectives
The main objective of this project is to create a stabilizing mechanism that can reduce undesired movements and vibrations. By utilizing a microcontroller and servo motor, the aim is to build a prototype that can effectively stabilize motion. The project’s results could lead to further research and development of improved and cost-effective stabilizing solutions.

## Components Used
2x Servo Motor SG90
Gyroscope Module MPU6050
Jumper Wires
Arduino UNO R3
3D-printed Plastic Components

## Servo Motor 90SG
The motor we are using is the 90SG servo motor, which is a standard electric motor equipped with servo control mechanism. This motor is a rotary actuator that allows precise control over angular or linear position.
The SG90 servo motor has three pins: S, Vcc, and GND. The S pin is the signal pin that connects to the PWM pin of the Arduino. The +ve and -ve pins connect to 5 volts and GND on the Arduino, respectively. The wiring scheme for the servo motor is provided in Figure 2.
<div style="display: flex;">
    <img src="Data\Picture1.png" alt="Image 1" style="width: 30%;">
    <img src="Data\Picture2.png" alt="Image 2" style="width: 30%;">
</div>

The connection of the motor to the Arduino is illustrated in Figure 3.
<div style="display: flex;">
    <img src="Data\Picture3.png" alt="Image 1" style="width: 100%;">
</div>
Specifications of Motor 90SG:
Operating Voltage: 4.8 to 6 volts (typically 5 volts)
Stall Torque: 1.8 kg/cm (at 4.8 volts)
Maximum Stall Torque: 2.2 kg/cm (at 6 volts)
Operating Speed: 0.1 sec/60 degrees (at 4.8 volts)
Rotation: 180-0 degrees
Motor Weight: 13.4 grams

## Sensor Analysis
Now, let’s delve into the sensor used in the robot. Our selected sensor is the MPU6050 module, which is a gyroscope type sensor. This module is a 6-axis motion tracking device that combines a 3-axis gyroscope, a 3-axis accelerometer, and a digital motion processor in a compact package. Additionally, it features an onboard temperature sensor. It has an I2C interface for communication with microcontrollers.
<div style="display: flex;">
    <img src="Data\Picture4.png" alt="Image 1" style="width: 100%;">
</div>
The MPU6050 sensor provides precise motion tracking capabilities, making it suitable for applications requiring accurate movement monitoring and control. Its compact design and integrated features make it a versatile choice for robotics and motion control projects.
<div style="display: flex;">
    <img src="Data\Picture5.png" alt="Image 1" style="width: 100%;">
</div>

## Noise Filter
### Moving Average Filter
The Moving Average Filter is one of the simplest types of filters. It works by taking a specified number of consecutive data points, averaging them, and using this average as a replacement. The moving average filter is a simple technique that creators can use to smooth out their signals, remove noise, and make sensor output easier to learn from. Passing a signal through this filter removes higher-frequency information from the output. While a traditional low-pass filter can effectively focus on the desired signal frequency, the moving average filter provides a more direct approach to “smoothing” the signal.
The only real parameter that can be controlled in the moving average filter is the window size, which is the number of previous signal input values that can be averaged together. If the window is too small, the signal may still contain a lot of noise. However, if the window is too large, vital information in the signal may be lost. Choosing the appropriate window size is a trial-and-error process.
The working principle of the code is such that it sums up a number of data points, places them in a dataset, and arranges them in a way that the newest data point enters while the oldest one exits. This continuous averaging process helps in smoothing out the signal and reducing noise in the data.

Moving Average Filter Analysis:
According to the code, if we change the window_size, the cutoff frequency for the signals will change. This means that if the number of data points averaged is too high, a significant portion of the signal will not pass through, resulting in an inappropriate value and lag. Conversely, if the window size is too small, the system noise will not be effectively removed. Therefore, finding the optimal value is crucial.
We can place values of 5, 10, 20, and 100 for the window_size and analyze the results. 
<div style="display: flex;">
    <img src="Data\Picture6.png" alt="Image 1" style="width: 100%;">
</div>
The best case was observed with a window_size of 20. In this scenario, it is noted that the noise level is minimized, and at the same time, the signal range is appropriate. Adjusting the window_size to 20 resulted in an optimal balance between noise reduction and signal fidelity.


### Kalman Filter
The Kalman Filter is an estimator that uses the estimation of the previous state and the current observation to calculate the estimation of the current state. It is a powerful tool for combining information in the presence of uncertainties. Wherever there is uncertainty about a dynamic system, the Kalman Filter can provide a suitable estimate of the system’s future changes. Kalman filters are ideal for systems that are constantly changing.
The advantage of Kalman filters is that they require minimal memory because they do not need memory except for storing previous state information. Additionally, these filters are very fast, making them suitable for real-time issues and embedded systems.

Kalman Filter Results Analysis:
We will examine the results of the Kalman filter for different values of p_mea, p_est, and q as provided in the table. According to the Kalman equations, three variables are specified to determine its functionality as follows:
  SimpleKalmanFilter(e_mea, e_est, q)
- e_mea: Measurement Uncertainty, indicating the level of uncertainty for the received value.
- e_est: Estimated Uncertainty, representing the uncertainty level for the predicted value by the Kalman filter.
- q: The periodicity of noise that we want to remove from the signal.
By adjusting these parameters, we can fine-tune the Kalman filter to optimize its performance in terms of noise reduction and signal estimation accuracy. Analyzing the results for different combinations of p_mea, p_est, and q values will provide insights into how these parameters impact the filter’s behavior and effectiveness in handling uncertainties and noise in the system.

Increasing the level of uncertainty leads to a wider range between the input and output, resulting in a more stable output signal with reduced oscillations. However, this can introduce lag and dynamic errors into the system.
On the other hand, decreasing uncertainty reduces dynamic errors but leaves more noise in the system.
<div style="display: flex;">
    <img src="Data\Picture7.png" alt="Image 1" style="width: 100%;">
</div>
The most suitable parameters were found to be e_mea=2, e_est=2, and q=0.001 as input values. These parameters strike a balance between stability, noise reduction, and dynamic error minimization, resulting in an optimized performance of the Kalman filter for the given system.
