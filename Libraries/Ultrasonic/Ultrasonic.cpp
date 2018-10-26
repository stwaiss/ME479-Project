/* 
*
*
*
*
*
*/
#include "Arduino.h"
#include "Ultrasonic.h"

//Define default constructor	
Ultrasonic::Ultrasonic(){}	

//Define constructor with 3 integer parameters
Ultrasonic::Ultrasonic(int signal){
	int signalPin = signal;
	long metrics[] = {0,0};			//duration, inches  


	pinMode(signalPin, INPUT);

}


//Define all public methods and their routines

long Ultrasonic::microsecondsToInches(long microseconds) {
	  // According to Parallax's datasheet for the PING))), there are 73.746
	  // microseconds per inch (i.e. sound travels at 1130 feet per second).
	  // This gives the distance travelled by the ping, outbound and return,
	  // so we divide by 2 to get the distance of the obstacle.
	  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
	  
	  return microseconds / 74 / 2;
}


void Ultrasonic::ping(){
	  long duration, inches;

	  // The PING is triggered by a HIGH pulse of 2 or more microseconds.
	  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	  pinMode(signalPin, OUTPUT);
	  digitalWrite(signalPin, LOW);
	  delayMicroseconds(2);
	  digitalWrite(signalPin, HIGH);
	  delayMicroseconds(5);
	  digitalWrite(signalPin, LOW);

	  // The same pin is used to read the signal from the PING))): a HIGH pulse
	  // whose duration is the time (in microseconds) from the sending of the ping
	  // to the reception of its echo off of an object.
	  pinMode(signalPin, INPUT);
	  duration = pulseIn(signalPin, HIGH);

	  // convert the time into a distance
	  inches = microsecondsToInches(duration);

	  metrics[0] = duration;
	  metrics[1] = inches;

	  return;

}

long Ultrasonic::getDuration(){
	return metrics[0];
}


long Ultrasonic::getInches(){
	return metrics[1];
}

