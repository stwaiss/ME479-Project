/*
*
*
*
*/

#ifndef Clock_h
#define Clock_h

#include "Arduino.h"


class Clock{

	//declare private variables
	private:	
	int hours;
	int minutes;
	int seconds;


	//define public constructors and methods
	public:
	Clock();
	Clock(int hrs, int min, int sec);

	void incrementTime();

	int getSeconds();
	int getMinutes();
	int getHours();

};
#endif