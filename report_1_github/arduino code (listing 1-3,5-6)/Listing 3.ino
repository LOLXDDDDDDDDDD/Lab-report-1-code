#include <ESP32Encoder.h>

//enable encoder
ESP32Encoder encoder;

void setup() {
    Serial.begin(115200);

    // Enable the weak pull up resistors for encoder
    ESP32Encoder::useInternalWeakPullResistors = UP;

    // use pin 18 and 19 for the encoder
    encoder.attachHalfQuad(18, 19);

    // clear the encoder's raw count and set the tracked count to zero
    encoder.clearCount();
}

void loop() {

    //count encoder
    Serial.println("Encoder count = " + String((int32_t)encoder.getCount()));
    distance = encoder.getCount();
    delay(5);

    //loop count +1
    loop_count++;
}