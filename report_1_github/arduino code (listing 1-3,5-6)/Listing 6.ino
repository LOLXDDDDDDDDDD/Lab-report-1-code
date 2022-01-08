#include <Wire.h>
#include <WireSlave.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

void receiveEvent(int howMany);

void setup() {
    Serial.begin(115200);

    bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
    if (!success) {
        Serial.println("I2C slave init failed");
        while (1)
            delay(100);
    }

    WireSlave.onReceive(receiveEvent);
}

void loop() {
    
    //update data recieved from master
    WireSlave.update();

    // let I2C and other ESP32 peripherals interrupts work
    delay(1);
}

void receiveEvent(int howMany) {
    while (1 < WireSlave.available()) // loop through all but the last byte
    {
        char c = WireSlave.read(); // receive byte as a character
        Serial.print(c);           // print the character
    }

    int x = WireSlave.read(); // receive byte as an integer
    Serial.println(x);        // print the integer
}
