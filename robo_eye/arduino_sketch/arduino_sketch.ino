#include <Servo.h>

Servo servo_x;
Servo servo_y;

int value = 0;

void setup() {
  Serial.begin(9600);
  
  servo_x.attach(2);
  servo_y.attach(4);
}

void loop() {

    while (Serial.available() > 0) {
        delay(15);
        int c = Serial.read();
        if (c >= '0' && c <= '9') {
            value = 10 * value + (c - '0');
        } else if (c == '\n') {
            servo_y.write(value);
            Serial.print("\tY: ");
            Serial.println(value);
            value = 0;
        } else if (c == '\r') {
            // skip
        } else if (c == '\t') {
            servo_x.write(value);
            Serial.print("X: ");
            Serial.print(value);
            value = 0;
        } else {
            // reset on any unexpected character
            value = 0;
        }
    }

}
