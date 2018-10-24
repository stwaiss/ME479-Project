	/* 
	*
	*
	*
	*
	*
	*/
	#include "Drivetrain.h"
	#include "Arduino.h"
	

	//Define default constructor	
	StationBay::StationBay(){}	

	//Define constructor with 3 integer parameters
	StationBay::StationBay(int rp, int pp, int lcd){
		resetPin = rp;
		powerPin = pp;
		LCDPin = lcd;
		cycleCount = 0;
		powerStatus = 0;
		isStuck = 0;
		stationTimer = 0;
		timesIsStuck = 0;

		pinMode(resetPin, INPUT);
		pinMode(powerPin, INPUT);
		pinMode(LCDPin, INPUT);

		digitalWrite(resetPin, LOW);
		digitalWrite(powerPin, LOW);
		digitalWrite(LCDPin, LOW);
	}
	
	//Define constructor with 4 integer parameters
	StationBay::StationBay(int rp, int pp, int lcd, long cc){
		resetPin = rp;
		powerPin = pp;
		LCDPin = lcd;
		cycleCount = cc;
		powerStatus = 0;
		isStuck = 0;
		stationTimer = 0;
		timesIsStuck = 0;
				
		pinMode(resetPin, INPUT);
		pinMode(powerPin, INPUT);
		pinMode(LCDPin, INPUT);

		digitalWrite(resetPin, LOW);
		digitalWrite(powerPin, LOW);
		digitalWrite(LCDPin, LOW);
	}
	

	//Define all public methods and their routines
	long StationBay::getCycleCount(){return cycleCount;}

	int StationBay::getPowerStatus(){return powerStatus;}

	int StationBay::getIsStuck(){return isStuck;}

	int StationBay::getTimesIsStuck(){return timesIsStuck;}

	unsigned long StationBay::getStationTimer(){return stationTimer;}

	int StationBay::getPowerPin(){return powerPin;}

	int StationBay::getResetPin(){return resetPin;}

	int StationBay::getLCDPin(){return LCDPin;}

	void StationBay::incrementCycleCount(){cycleCount++;}

	void StationBay::resetCycleCount(){cycleCount = 0;}

	void StationBay::powerStatusOn(){powerStatus = 1;}

	void StationBay::powerStatusOff(){powerStatus = 0;}

	void StationBay::setIsStuckTrue(){isStuck = 1;}

	void StationBay::setIsStuckFalse(){isStuck = 0;}

	void StationBay::resetTimesIsStuck(){timesIsStuck = 0;}

	void StationBay::incrementTimesIsStuck(){timesIsStuck++;}

	void StationBay::setStationTimer(long st){stationTimer = st;}