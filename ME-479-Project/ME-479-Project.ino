/*
 * 
 */

// Include all necessary libraries
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Drivetrain.h>
#include <Ultrasonic.h>
#include <Clock.h>
#include <Keypad.h>
#include <TimerOne.h>

// Define various pin constants;
const int eButtonPin = 100;
const int sensorFL_Pin = 2;
const int sensorFR_Pin = 3;
const int leftMotorPin = 5;
const int rightMotorPin = 6;

// Define necessary information used for the keypad

// Define objects used throughout program
Drivetrain drivetrain(leftMotorPin, rightMotorPin);
Ultrasonic sensorFL(sensorFL_Pin);
Ultrasonic sensorFR(sensorFR_Pin);


void setup() {
  Serial.begin(9600);
  
}

void loop() {


       drivetrain.driveForward(100);

       delay(1000);

       drivetrain.stop();

       delay(1000);

      // Ping ultrasonic sensors to check if something is in the way.
//       sensorFL.ping();
//       sensorFR.ping();
//
//       Serial.print("SensorFL: ");
//       Serial.print(sensorFL.getDuration());
//
//       Serial.print(" SensorFR: ");
//       Serial.println(sensorFR.getDuration());
//       if(sensorFL.getDuration() < 700 || sensorFR.getDuration() < 700){
//          //drivetrain.turn90();
//          //drivetrain.driveForward(150);
//       }

       delay(1000);
}


