/* 
*
*
*
*
*
*/
#include "Drivetrain.h"
#include "Arduino.h"
#include "Servo.h"

//Define default constructor	
Drivetrain::Drivetrain(){}	

//Define constructor with 2 integer parameters
Drivetrain::Drivetrain(int lmp, int rmp){
	int leftMotorPin = lmp;
	int rightMotorPin = rmp;

	Servo leftMotor;
	Servo rightMotor;

	//Define digital pin modes
	pinMode(leftMotorPin, OUTPUT);
	pinMode(rightMotorPin, OUTPUT);

	leftMotor.attach(lmp);                       //Set up Servo Outputs on pins defined above, initialize microstep
	rightMotor.attach(rmp); 

	int leftOutput = 90;                                              //Define Outputs for Servo outputs to Sabertooth
	int rightOutput = 90;

}


//Define all public methods and their routines

void Drivetrain::driveForward(int speed){
	
	leftMotor.write(speed);
	Serial.println("In Drive Forward");
	rightMotor.write(speed);
	
}

void Drivetrain::turn90(){
	leftMotor.write(180);
	Serial.println("In Drive Turn");
	rightMotor.write(0);

	delay(375);
}

void Drivetrain::stop(){
	leftMotor.write(90);
	Serial.println("In Drive Stop");
	rightMotor.write(90);

	delay(1000);
}

