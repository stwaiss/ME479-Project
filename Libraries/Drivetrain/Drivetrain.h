/*
*
*
*
*/

#ifndef Drivetrain_h
#define Drivetrain_h

#include "Arduino=.h"

class Drivetrain{

	//declare private variables
	private:
	int resetPin;
	int powerPin;
	int LCDPin;
	volatile unsigned long cycleCount;
	volatile int powerStatus;
	volatile int isStuck;
	volatile int timesIsStuck;
	volatile unsigned long stationTimer;
	
	//define public constructors and methods
	public:
	StationBay();
	StationBay(int rp, int pp, int lcd);
	StationBay(int rp, int pp, int lcd, long cc);
	long getCycleCount();
	int getPowerStatus();
	int getIsStuck();
	unsigned long getStationTimer();
	int getTimesIsStuck();
	int getPowerPin();
	int getResetPin();
	int getLCDPin();

	void incrementCycleCount();
	void resetCycleCount();
	void powerStatusOff();
	void powerStatusOn();
	void setIsStuckTrue();
	void setIsStuckFalse();
	void resetTimesIsStuck();
	void incrementTimesIsStuck();
	void setStationTimer(long st);
};
#endif