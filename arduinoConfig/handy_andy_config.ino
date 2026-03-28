/*
Initial testing and calibration of servo motors connected to the
Adeept HAT, which is wired to the Arduino Mega 2560 R3 board.

Connections from HAT P1 Header to Arduino:
Pin 1 (3.3V) -> 3.3V
Pin 3 (SDA1) -> 20 (SDA)
Pin 5 (SCL1) -> 21 (SCL)
Pin 6 (GND)  -> GND
Referenced from Adeept_RaspClaws-V2.0-20251030/Circuit-Schematic-Adeept_Robot-HAT.pdf
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Default I2C address for the PCA9685 is 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Standard servo pulse widths
#define SERVOMIN  200
#define SERVOMAX  580

int fingerServos[4] = {0, 1, 2, 3};
int splayServos[3] = {4, 5, 6};
int thumbServos[2] = {7, 8};

int indexFinger = fingerServos[0];
int middleFinger = fingerServos[1];
int ringFinger = fingerServos[2];
int pinkieFinger = fingerServos[3]; 

// Each finger has a different length
float fingerProportionality[4] = {1.0, 1.0, 1.0, 1.0};

float angle = 0;

struct ServoConfig {
  int port;
  int minPulse = SERVOMIN;
  int maxPulse = SERVOMAX;
  int offset = 0;
};

ServoConfig servos[9] = {
  ServoConfig(.port=0), ServoConfig(.port=1), ServoConfig(.port=2), ServoConfig(.port=3),
  ServoConfig(.port=4), ServoConfig(.port=5), ServoConfig(.port=6),
  ServoConfig(.port=7), ServoConfig(.port=8)
};

// TODO: implement more helper functions
// float calcPulse()

// Writes an angle to the servo, 0 <= degree <= 160
void writeServo(int servoIndex, float degree) {
  ServoConfig s = servos[servoIndex];
  if (degree < 0.0) {
    degree = 0.0;
  } else if (degree > 160.0) {
    degree = 160.0;
  }
  float exactPulse = (degree - 0.0) * (s.maxPulse - s.minPulse) / 160.0 + s.minPulse;
  int pulse = round(exactPulse);
  pwm.setPWM(s.port, 0, pulse);
}

// Moves a servo based on percentage
void moveServoPercent(int servoIndex, float percent) {
  ServoConfig s = servos[servoIndex];
  if (percent < 0.0) {
    percent = 0.0;
  } else if (percent > 100.0) {
    percent = 100.0;
  }
  float exactPulse = s.minPulse + percent * (s.maxPulse - s.minPulse) / 100.0;
  int pulse = round(exactPulse);
  pwm.setPWM(s.port, 0, pulse);
}

// Moves a finger (0..3)
void setFinger(int fingerIndex, float percent) {
  moveServoPercent(fingerServos[fingerIndex], percent);
}

void setSplay(int leftFinger, int rightFinger, float percent) {
  if (leftFinger == pinkieFinger || rightFinger == indexFinger || rightFinger - leftFinger != 1) {
    return;
  }

  if (leftFinger == indexFinger) {
    moveServoPercent(splayServos[0], percent);
  } else if (leftFinger == middleFinger) {
    moveServoPercent(splayServos[1], percent);
  } else if (leftFinger == ringFinger) {
    moveServoPercent(splayServos[2], percent);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Running...");

  pwm.begin();
  pwm.setOscillatorFrequency(25000000);
  pwm.setPWMFreq(50);  // Analog servos run at ~50 Hz
  delay(10);
}

void loop() {

  if (Serial.available() > 0) {
    angle = Serial.parseFloat();
    for (int i = 0; i < 2; i++) {
      writeServo(i, angle);
    }
    
  } 
}
