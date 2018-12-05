// Include all necessary libraries
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

// Define various pin constants;
const int eButtonPin = 100;
const int sensor_Pin = 2;
const int leftMotorPin = 5;
const int rightMotorPin = 6;

Servo leftMotor;
Servo rightMotor;

// Define necessary information used for the keypad

void setup() {
  //Define digital pin modes
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

  leftMotor.attach(leftMotorPin);                       //Set up Servo Outputs on pins defined above, initialize microstep
  rightMotor.attach(rightMotorPin); 
  Serial.begin(9600);
  
}

void loop() {
  drivetrainDrive(125);

// Ping ultrasonic sensors to check if something is in the way.
   int usDuration = ultrasonicPing(sensor_Pin);

   Serial.print("Ulrasonic Sensor: ");
   Serial.println(usDuration);
   if(usDuration < 2000){
      drivetrainTurn90();
      drivetrainDrive(125);
   }

  delay(100);
       
}

//****************************************************************************************

void drivetrainDrive(int speed){
  leftMotor.write(speed);
  rightMotor.write(speed);
}

void drivetrainStop(){
  leftMotor.write(90);
  rightMotor.write(90);
}

void drivetrainTurn90(){
  leftMotor.write(150);
  rightMotor.write(30);
  delay(500);
}

//*************************************************************************
int ultrasonicPing(int signalPin){
    int duration = 0;
    // The PING is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(signalPin, OUTPUT);
    digitalWrite(signalPin, LOW);
    delayMicroseconds(2);
    digitalWrite(signalPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(signalPin, LOW);

    // The same pin is used to read the signal from the PING))): a HIGH pulse
    // whose duration is the time (in microseconds) from the sending of the ping
    // to the reception of its echo off of an object.
    pinMode(signalPin, INPUT);
    duration = (int) pulseIn(signalPin, HIGH);

    return duration;
}






