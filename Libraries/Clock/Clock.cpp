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


void Clock::incrementTime(){
	  time[2] = time[2]++;         	//increment ms position

	  if(time[2] == 60){            //if s = 60, roll over to 0
	    time[2] = 0;                //reset s position
	    time[1] = time[1]++;        //increment min position
	  }

	  if(time[1] == 60){            //if min = 60, roll over to 0
	    time[1] = 0;                //reset min position
	    time[0] = time[0]++;      		//increment hr position
	  }

	  if(time[0] == 24){
	  	time[0] == 0;
	  }
}

void Clock::setSeconds(int s){time[2] = s;}

void Clock::setMinutes(int m){ time[1] = m;}

void Clock::setHours(int h){time[0] = h;}

int Clock::getSeconds(){return time[2];}

int Clock::getMinutes(){return time[1];}

int Clock::getHours(){return time[0];}

boolean Clock::compareTimes(Clock c){

	//check if hours, minutes, and seconds are equal, and return true if equal and false if unequal
	boolean state = (time[0] == c.getHours() && time[1] == c.getMinutes() && time[2] == c.getSeconds()) ? true : false;

	return state;
}