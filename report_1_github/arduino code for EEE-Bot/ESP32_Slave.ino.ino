#include <Wire.h>
#include <WireSlave.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

//motor pin
#define r_blk 14
#define r_red 27
#define l_red 26
#define l_blk 25

void receiveEvent(int howMany);

const int freq = 5000;
const int pwm_r_red = 0;
const int pwm_r_blk = 1;
const int pwm_l_red = 2;
const int pwm_l_blk = 3;
const int resolution = 8;

void setup() {
    Serial.begin(115200);

    //initialize as slave
    bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
    if (!success) {
        Serial.println("I2C slave init failed");
        while (1)
            delay(100);
    }

    pinMode(r_blk, OUTPUT);
    pinMode(r_red, OUTPUT);
    pinMode(l_blk, OUTPUT);
    pinMode(l_red, OUTPUT);

    //setup pwm channel
    ledcSetup(pwm_l_blk, freq, resolution);
    ledcSetup(pwm_l_red, freq, resolution);
    ledcSetup(pwm_r_blk, freq, resolution);
    ledcSetup(pwm_r_red, freq, resolution);

    //attach pwm channel
    ledcAttachPin(l_blk, pwm_l_blk);
    ledcAttachPin(l_red, pwm_l_red);
    ledcAttachPin(r_blk, pwm_r_blk);
    ledcAttachPin(r_red, pwm_r_red);

    WireSlave.onReceive(receiveEvent);
}

void loop() {

    //update from master
    WireSlave.update();

    // let I2C and other ESP32 peripherals interrupts work
    delay(1);
}

// function that executes whenever a complete and valid packet is received from master
void receiveEvent(int howMany) {

    //reading distance is for debugging
    while (1 < WireSlave.available()) { // loop through all but the last byte
        Serial.print("distance=");      // receive byte as a character
        int d = WireSlave.read();       // receive byte as a character
        Serial.println(d);
    }

    int x;

    x = WireSlave.read(); // receive byte as an integer

    //different drive mode
    switch (x) {

        case 0:
            //go foward
            ledcWrite(pwm_l_blk, 0);
            ledcWrite(pwm_l_red, 245);
            ledcWrite(pwm_r_blk, 0);
            ledcWrite(pwm_r_red, 245);
            Serial.println("foward");
            break;

        case 1:
            //go backward
            ledcWrite(pwm_l_blk, 245);
            ledcWrite(pwm_l_red, 0);
            ledcWrite(pwm_r_blk, 245);
            ledcWrite(pwm_r_red, 0);
            Serial.println("backward");
            break;

        case 2:
            //go left
            ledcWrite(pwm_l_blk, 0);
            ledcWrite(pwm_l_red, 250);
            ledcWrite(pwm_r_blk, 160);
            ledcWrite(pwm_r_red, 0);
            Serial.println("left");
            break;

        case 3:
            //go right
            ledcWrite(pwm_l_blk, 160);
            ledcWrite(pwm_l_red, 0);
            ledcWrite(pwm_r_blk, 0);
            ledcWrite(pwm_r_red, 250);
            Serial.println("right");
            break;

        case 4:
            //stop
            ledcWrite(pwm_l_blk, 0);
            ledcWrite(pwm_l_red, 0);
            ledcWrite(pwm_r_blk, 0);
            ledcWrite(pwm_r_red, 0);
            Serial.println("stop");
            break;

        default:
            //stoping the car in case of choas
            ledcWrite(pwm_l_blk, 0);
            ledcWrite(pwm_l_red, 0);
            ledcWrite(pwm_r_blk, 0);
            ledcWrite(pwm_r_red, 0);
            Serial.println("ERROR");
            break;
    }
}