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
	int time[3];


	//define public constructors and methods
	public:
	Clock();

	void incrementTime();
	void setSeconds(int s);
	void setMinutes(int m);
	void setHours(int h);


	int getSeconds();
	int getMinutes();
	int getHours();
	boolean compareTimes(Clock c);

};
#endif