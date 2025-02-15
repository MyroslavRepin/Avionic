#include "FlightSoftware.h"

FlightSoftware::FlightSoftware() : mpu6050(Wire) {}

void FlightSoftware::begin() {
    Serial.begin(115200);
    Wire.begin();

    mpu6050.begin();
    mpu6050.calcGyroOffsets();
    Serial.println("Sensor is ready");

    servo_1.attach(6);
    servo_2.attach(3);
    servo_3.attach(10);
    servo_4.attach(11);

    servo_1_const = 96;
    servo_2_const = 99;
    servo_3_const = 83;
    servo_4_const = 86;

    constant = 5;

    servo_1.write(servo_1_const);
    servo_2.write(servo_2_const);
    servo_3.write(servo_3_const);
    servo_4.write(servo_4_const);
}

void FlightSoftware::update() {
    mpu6050.update();

    int Yaw = round(mpu6050.getAngleZ());
    int Pitch = round(mpu6050.getAngleY());
    int Roll = round(mpu6050.getAngleX());

    int filteredYaw = kalmanZ.getAngle(Yaw, mpu6050.getGyroZ(), 0.001);
    int filteredPitch = kalmanY.getAngle(Pitch, mpu6050.getGyroY(), 0.001);
    int filteredRoll = kalmanX.getAngle(Roll, mpu6050.getGyroX(), 0.001);

    int GyroZ = round(mpu6050.getGyroZ() / constant);
    int GyroY = round(mpu6050.getGyroY() / constant);
    int GyroX = round(mpu6050.getGyroX() / constant);

    printSensorData(Yaw, Pitch, Roll, GyroZ, GyroY, GyroX);
    processMovement(GyroZ);

    digitalWrite(13, HIGH);
    delay(20);
}

void FlightSoftware::processMovement(int GyroZ) {
    if (GyroZ > 25) {
        Serial.println("Servo turn limit (Positive)");
        servo_1.write(20 + servo_1_const);
        servo_2.write(20 + servo_2_const);
        servo_3.write(20 + servo_3_const);
        servo_4.write(20 + servo_4_const);
    }
    else if (GyroZ < -25) {
        Serial.println("Servo turn limit (Negative)");
        servo_1.write(-20 + servo_1_const);
        servo_2.write(-20 + servo_2_const);
        servo_3.write(-20 + servo_3_const);
        servo_4.write(-20 + servo_4_const);
    }
    else {
        servo_1.write(GyroZ + servo_1_const);
        servo_2.write(GyroZ + servo_2_const);
        servo_3.write(GyroZ + servo_3_const);
        servo_4.write(GyroZ + servo_4_const);
    }
}

void FlightSoftware::printSensorData(int Yaw, int Pitch, int Roll, int GyroZ, int GyroY, int GyroX) {
    char buffer[80];
    sprintf(buffer, "Yaw: %2d Gyro: %2d °/s | Pitch: %2d Gyro: %2d °/s | Roll: %2d Gyro: %2d °/s ",
            Yaw, GyroZ, Pitch, GyroY, Roll, GyroX);
    Serial.println(buffer);
}
