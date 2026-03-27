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

int fingerServos[] = {0, 1, 2, 3};
int splayServos[] = {4, 5, 6};
int thumbServos[] = {7, 8};

// Each finger has a different length
float fingerProportionality[] = {1.0, 1.0, 1.0, 1.0};

float angle = 0;

// Writes an angle to the servo, 0 <= degree <= 160
void writeServo(int port, int degree) {
  degree = constrain(degree, 0, 160);
  float exactPulse = (degree - 0.0) * (SERVOMAX - SERVOMIN) / 160.0 + SERVOMIN;
  int pulse = round(exactPulse);
  Serial.println(pulse);
  pwm.setPWM(port, 0, pulse);
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
