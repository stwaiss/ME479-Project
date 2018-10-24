/* 
*
*
*
*
*
*/
#include "Arduino.h"
#include "Clock.h"

//Define default constructor	
Clock::Clock(){}	

//Define constructor with 2 integer parameters
//
Clock::Clock(int hrs, int min, int sec){
	int hours = hrs;
	int minutes = min;
	int seconds = sec;

}


void Clock::incrementTime(){
	  seconds = seconds++;         	//increment ms position

	  if(seconds == 60){            //if s = 60, roll over to 0
	    seconds = 0;                //reset s position
	    minutes = minutes++;        //increment min position
	  }

	  if(minutes == 60){            //if min = 60, roll over to 0
	    minutes = 0;                //reset min position
	    hours = hours++;      		//increment hr position
	  }

	  if(hours == 24){
	  	hours == 0;
	  }
}


int Clock::getSeconds(){return seconds;}

int Clock::getMinutes(){return minutes;}

int Clock::getHours(){return hours;}