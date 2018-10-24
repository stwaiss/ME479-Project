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

void Drivetrain::drive(int speed, int rotation){
	if(rotation == 0){
		leftMotor.write(speed);
		rightMotor.write(speed);			
	}	

	else{
		//implement code to turn

		//Normalize rotation input so that only positive values between 0 and 360 are acceptable
		//and that rotation is measured clock-wise from the direction the robot is currently facing 
		rotation = abs(rotation);
		int multiplier = rotation/360;
		int turnDegrees = rotation - multiplier*360; 
	}

	return;
}

void Drivetrain::stop(){
	leftMotor.write(90);
	rightMotor.write(90);
}

