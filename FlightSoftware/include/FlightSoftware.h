#ifndef FLIGHT_SOFTWARE_H
#define FLIGHT_SOFTWARE_H

#include <Servo.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Kalman.h>

class FlightSoftware {
public:
    FlightSoftware();
    void begin();
    void update();

private:
    MPU6050 mpu6050;
    Servo servo_1, servo_2, servo_3, servo_4;
    Kalman kalmanX, kalmanY, kalmanZ;

    int servo_1_const, servo_2_const, servo_3_const, servo_4_const;
    int constant;

    void processMovement(int GyroZ);
    void printSensorData(int Yaw, int Pitch, int Roll, int GyroZ, int GyroY, int GyroX);
};

#endif
