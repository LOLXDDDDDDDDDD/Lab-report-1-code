#include <MPU6050_tockn.h>
#include <Wire.h>

#define led_pin 6 //D6

MPU6050 mpu6050(Wire);

long timer = 0;

void setup() {
    Serial.begin(115200);
    pinMode(led_pin, OUTPUT);
    Wire.begin();
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
}

void loop() {
    mpu6050.update();

    if (millis() - timer > 500) {

        Serial.println("=======================================================");
        Serial.print("angleX : ");
        Serial.print(mpu6050.getAngleX());
        Serial.print("\tangleY : ");
        Serial.print(mpu6050.getAngleY());
        Serial.print("\tangleZ : ");
        Serial.println(mpu6050.getAngleZ());
        Serial.println("=======================================================\n");
        timer = millis();
    }

    if ((mpu6050.getAngleZ() > 90) || (mpu6050.getAngleZ() < -90)) {
        digitalWrite(led_pin, HIGH);
    } else
        digitalWrite(led_pin, LOW);
}