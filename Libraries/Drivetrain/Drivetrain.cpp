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

//Define constructor with 3 integer parameters
Drivetrain::Drivetrain(int lmp, int rmp){
	int leftMotorPin = lmp;
	int rightMotorPin = rmp;

	Servo leftMotor;
	Servo rightMotor;

	//Define digital pin modes
	pinMode(leftMotorPin, OUTPUT);
	pinMode(rightMotorPin, OUTPUT);

	leftMotor.attach(lmp,1000,2000);                       //Set up Servo Outputs on pins defined above, initialize microstep
	rightMotor.attach(rmp,1000,2000); 

	int leftOutput = 90;                                              //Define Outputs for Servo outputs to Sabertooth
	int rightOutput = 90;

}


//Define all public methods and their routines

void Drivetrain::driveForward(int speed){
	
	leftMotor.write(speed);
	rightMotor.write(speed);
	
}

void Drivetrain::turn90(){
	leftMotor.write(180);
	rightMotor.write(0);

	delay(375);
}

void Drivetrain::stop(){
	leftMotor.write(90);
	rightMotor.write(90);
}

