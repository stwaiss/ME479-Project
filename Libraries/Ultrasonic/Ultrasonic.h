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
	long[] metrics;


	//define public constructors and methods
	public:
	Ultrasonic();
	Ultrasonic(int signal);

	void ping();
	long microsecondsToInches(long microseconds);

};
#endif