/*
 * 
 */

// Include all necessary libraries
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Drivetrain.h>
#include <Ultrasonic.h>
#include <Clock.h>
#include <Keypad.h>
#include <TimerOne.h>

// Define various pin constants;
const int eButtonPin = 100;
const int sensorFL_Pin = 101;
const int sensorFR_Pin = 102;
const int sensorRL_Pin = 103;
const int sensorRR_Pin = 104;
const int leftMotorPin = 105;
const int rightMotorPin = 106;

// Define necessary information used for the keypad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

// Define objects used throughout program
LiquidCrystal LCD(1,2,3,4,5,6);
Clock currentClock;
Clock alarmClock;
Drivetrain drivetrain(leftMotorPin, rightMotorPin);
Ultrasonic sensorFL(sensorFL_Pin);
Ultrasonic sensorFR(sensorFR_Pin);
Ultrasonic sensorRL(sensorRL_Pin);
Ultrasonic sensorRR(sensorRR_Pin);
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);



void setup() {
  // Initialize interrupt timer library for 1 million - milliseconds (1 second);
  Timer1.initialize(1000000); 
  
  // Set the Emergency Stop Button as an interrupt service routine
  attachInterrupt(digitalPinToInterrupt(eButtonPin),eButtonStop,RISING);

  // Initialize LCD screen and print start-up message
  LCD.begin(16,2);
  LCD.print("Hello!");
  
  setClocks(currentClock, "current time");

  Timer1.attachInterrupt(timerInterrupt);
  setClocks(alarmClock, "alarm clock");
  
}

void loop() {
  if(alarmClock.compareTimes(currentClock)){
    //Start sounding alarm noise

    //Begin driving in avoidance mode
    drivetrain.drive(150);
  }
}

//****************************************************************************************************
// This is the interrupt service routine for the timer that runs to keep the current time up to date
// in the background while the rest of the program runs

void timerInterrupt(){
  currentClock.incrementTime();
}



//****************************************************************************************************
// This is the interrupt service routine for when the Emergency Stop Button is depressed.
// It stops the drivetrain and exits.
void eButtonStop(){
  drivetrain.stop();
}


//*****************************************************************************************************
// This function takes user input from the keypad to set the current time and the time the alarm should go off.
void setClocks(Clock c, String clockName){
 
  LCD.clear();

  LCD.print("Enter " + clockName);
  LCD.setCursor(0,1);
  LCD.print("HH:MM:SS ");

  // Use counter to loop 3 times to input hours, minutes, seconds
  int counter = 0;

  //HH:MM:SS_!, where ! is position 7
  int cursorPosition = 7;
  
  // Accept input to set time
  while(counter < 3){

    // Set cursor to proper location
    LCD.setCursor(cursorPosition,1);

    //Grab new character from keypad and store as string
    String key1 = (String) keypad.getKey();

    // If key1 has a string, print it to screen and increment cursor position
    if (key1){
      LCD.print(key1);
      cursorPosition++;
    }

    // Repeat but for key2
    LCD.setCursor(cursorPosition,1);
    String key2 = (String) keypad.getKey();
    
    if (key2){
      LCD.print(key2);
      cursorPosition++;
    }

    // Concatenate the two key strings and convert into integer.
    // Based on counter, store to proper Clock variable. 
    if(counter == 0){
      c.setHours((key1 + key2).toInt());
    }
    else if(counter == 1){
      c.setMinutes((key1 + key2).toInt());
    }
    else{
      c.setSeconds((key1 + key2).toInt());
    }

    counter++;
  }

}

