/*
*
*
*
*/

#ifndef Drivetrain_h
#define Drivetrain_h

#include "Arduino.h"
#include "Servo.h"


class Drivetrain{

	//declare private variables
	private:
	Servo leftMotor;
	Servo rightMotor;	
	int leftMotorPin;
	int rightMotorPin;

	//define public constructors and methods
	public:
	Drivetrain();
	Drivetrain(int lmp, int rmp);

	void drive(int speed, int rotation);
	void stop();

};
#endif