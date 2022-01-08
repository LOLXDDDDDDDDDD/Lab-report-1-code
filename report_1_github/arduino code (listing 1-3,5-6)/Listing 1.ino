#include <Ultrasonic.h>

#define echoPin 11 // D11 
#define trigPin 12 //D12 
#define led_pin 6  //D6

Ultrasonic ultrasonic(trigPin, echoPin);

int distance;

void setup() {
    Serial.begin(9600);
    pinMode(led_pin, OUTPUT);
}

void loop() {

    distance = ultrasonic.read();

    Serial.print("Distance in CM: ");
    Serial.println(distance);
    analogWrite(led_pin, distance);

    delay(10);
}