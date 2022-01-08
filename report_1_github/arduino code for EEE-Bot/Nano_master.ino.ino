#include <MPU6050_tockn.h>
#include <Ultrasonic.h>
#include <Wire.h>
#include <WirePacker.h>

#define led_pin 6           //D6
#define echo_pin 11         //D11
#define trig_pin 12         //D12
#define I2C_SLAVE_ADDR 0x04 //slave adress

Ultrasonic ultrasonic(trig_pin, echo_pin);
MPU6050 mpu6050(Wire);

int turn_c;
int dist;

void write_slave(int i) {

    // first create a WirePacker that will assemble a packet
    WirePacker packer;

    // then add data the same way as you would with Wire
    packer.write(dist); //only for debugging, not affecting logic
    packer.write(i);
    Serial.println(i);

    // after adding all data wanting to send, close the packet
    packer.end();

    // now transmit the packed data
    Wire.beginTransmission(I2C_SLAVE_ADDR);
    while (packer.available()) { // write every packet byte
        Wire.write(packer.read());
    }

    Wire.endTransmission(); // stop transmitting
    delay(5);
}

void blink() {
    for (int i = 0; i <= 5; i++) {
        digitalWrite(led_pin, HIGH);
        delay(50);
        digitalWrite(led_pin, LOW);
        delay(50);
    }
}

void setup() {

    //starting things up
    pinMode(led_pin, OUTPUT);
    Wire.begin();
    mpu6050.begin();
    Serial.begin(9600);

    delay(450);

    //lights on when calibrating gyro
    blink();
    digitalWrite(led_pin, HIGH);
    delay(200);
    mpu6050.calcGyroOffsets();
    digitalWrite(led_pin, LOW);
}

void loop() {

    float new_angle; //new_angle is angle set at 0 after first turn when going backward

    mpu6050.update();
    delay(1000);
    dist = ultrasonic.read();

    //mode 0 for foward
    //mode 1 for backward
    //mode 2 for 90 degree turn left
    //mode 3 for 90 degree turn right
    //mode 4 for stopping

    switch (turn_c) {

        //going straight for 1 sec, then 180 no scope
        case 0:

            //straight for (10ms*t)
            for (int t = 0; t <= 100; t++) {
                write_slave(0);
                delay(5);
            }

            //stop the car
            write_slave(4);
            delay(30);
            digitalWrite(led_pin, HIGH);

            //180 no scope
            while (!((mpu6050.getAngleZ() < -180) || (mpu6050.getAngleZ() > 180))) {
                mpu6050.update();
                write_slave(3); //change this for L/R(2/3)
                delay(30);
            }

            //stop after 180
            write_slave(4);
            delay(500);
            digitalWrite(led_pin, LOW);

            //escape case
            turn_c++;

            break;

        //after the 180
        case 1:

            //gyro (z_angle) update and offset at 180/-180(since straight line so doesn't matter)
            mpu6050.update();
            if (mpu6050.getAngleZ() >= 0) {
                new_angle = (mpu6050.getAngleZ()) - 180;
            }

            else if (mpu6050.getAngleZ() <= 0) {
                new_angle = (mpu6050.getAngleZ()) + 180;
            }

            //go backward if nothing special
            write_slave(1);
            delay(30);

            //initiate turn 90 when dist<10
            if (dist <= 10) {

                digitalWrite(led_pin, HIGH); //turn on led till turn complete

                //stop and turn
                write_slave(4);
                delay(30);

                //turn 90 till 90 turned L/R, then break loop
                while (!((new_angle < -90) || (new_angle > 90))) {

                    //gyro (z_angle) offset at 180/-180 (updating in loop)
                    mpu6050.update();
                    if (mpu6050.getAngleZ() >= 0) {
                        new_angle = (mpu6050.getAngleZ()) - 180;
                    }

                    else if (mpu6050.getAngleZ() <= 0) {
                        new_angle = (mpu6050.getAngleZ()) + 180;
                    }

                    write_slave(3); //change this for L/R(2/3)
                    delay(15);
                }

                //turned and stop
                write_slave(4);
                delay(700);

                //escape loop
                turn_c++;

                digitalWrite(led_pin, LOW);
            }

            break;

        //after the 90 turn
        case 2:

            //go backfoward if nothing special
            write_slave(1);
            delay(30);

            //stop and move to next case when certain dist (in cm) between wall and car
            if (dist <= 10) {
                write_slave(4);
                delay(700);
                turn_c++;
            }

            break;

        //final moment (180 + 90 + near wall 2nd time)
        default:

            //stop the car forever
            write_slave(4);
            digitalWrite(led_pin, HIGH);
            delay(1000);
            digitalWrite(led_pin, LOW);
            delay(1000);
            break;
    }

    delay(5);
}
