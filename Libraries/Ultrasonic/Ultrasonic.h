/*
*
*
*
*/

#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "Arduino.h"


class Ultrasonic{

	//declare private variables
	private:	
	int signalPin;
	int duration;
	int inches;


	//define public constructors and methods
	public:
	Ultrasonic();
	Ultrasonic(int signal);

	long microsecondsToInches(long microseconds);
	void ping();

	int getDuration();
	int getInches();
};
#endif