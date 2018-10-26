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

void Drivetrain::drive(int speed){
	
	// If speed > 90, ramp up slowly to speed going forward
	if(speed > 90){
		for(int i = 90; i <= speed && i>=180; i = i+5){
			leftMotor.write(i);
			rightMotor.write(i);

			delay(50);
		}
	}

	// Else, if speed < 90, ramp up slowly to speed going backward
	else{
		for(int i = 90; i >= speed && i>=0; i = i-5){
			leftMotor.write(i);
			rightMotor.write(i);

			delay(50);
		}
	}
}

void Drivetrain::stop(){
	leftMotor.write(90);
	rightMotor.write(90);
}

