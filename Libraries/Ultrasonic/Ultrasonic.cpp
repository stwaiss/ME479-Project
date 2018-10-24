	/* 
	*
	*
	*
	*
	*
	*/
	#include "Arduino.h"

	//Define default constructor	
	Ultrasonic::Ultrasonic(){}	

	//Define constructor with 3 integer parameters
	Ultrasonic::Ultrasonic(int signal){
		int signalPin = signal;
		long metrics[] = {0,0}			//duration, inches  


		pinMode(signalPin, INPUT);

	}
	

	//Define all public methods and their routines

	void Ultrasonic::ping(){
		  long duration, inches;

		  // The PING is triggered by a HIGH pulse of 2 or more microseconds.
		  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
		  pinMode(pingPin, OUTPUT);
		  digitalWrite(pingPin, LOW);
		  delayMicroseconds(2);
		  digitalWrite(pingPin, HIGH);
		  delayMicroseconds(5);
		  digitalWrite(pingPin, LOW);

		  // The same pin is used to read the signal from the PING))): a HIGH pulse
		  // whose duration is the time (in microseconds) from the sending of the ping
		  // to the reception of its echo off of an object.
		  pinMode(pingPin, INPUT);
		  duration = pulseIn(pingPin, HIGH);

		  // convert the time into a distance
		  inches = microsecondsToInches(duration);

		  metrics[0] = duration;
		  metrics[1] = inches;

		  return;

	}

	long Ultrasonic::microsecondsToInches(long microseconds) {
		  // According to Parallax's datasheet for the PING))), there are 73.746
		  // microseconds per inch (i.e. sound travels at 1130 feet per second).
		  // This gives the distance travelled by the ping, outbound and return,
		  // so we divide by 2 to get the distance of the obstacle.
		  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
		  return microseconds / 74 / 2;
	}
