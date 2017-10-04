// Sweep
// by BARRAGAN <http://barraganstudio.com>
// This example code is in the public domain.

// modified 8 Nov 2013
 //by Scott Fitzgerald
 //http://www.arduino.cc/en/Tutorial/Sweep

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int servoPosition = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (servoPosition = 500; servoPosition <= 2500; servoPosition += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.writeMicroseconds(servoPosition);              // tell servo to go to position in variable 'pos'
    delay(1);                      // waits 15ms for the servo to reach the position
  }
  for (servoPosition = 2500; servoPosition >= 500; servoPosition -= 1) { // goes from 180 degrees to 0 degrees
    myservo.writeMicroseconds(servoPosition);              // tell servo to go to position in variable 'pos'
//    delay(1);                       // waits 15ms for the servo to reach the position
  }
  delay(750);
}
