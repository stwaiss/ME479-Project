/*
 * Alarm Clock Robot
 * 
 * Mechanical Engineering - 479, Introduction to Mechatronic Systems
 * Professor M. H. Rahman PhD
 * 
 * Semester Project - Group 3
 * Matthew Brook, Reid Rauchle, Tyler Schommer, Sean Waiss
 * 
 * This program runs the Lynxmotion Platform, powered by an Arduino Mega and Sabertooth
 * Motor Controller to serve as an Alarm Clock Robot. The User will be prompted to input
 * the current time and the time for which the alarm should go off, and then once the alarm
 * time is reached, the platform will drive around in avoidance mode and sound a tone. The 
 * user must solve a basic arithmetic problem to turn off the alarm and place the robot back 
 * in "Clock" mode.
 *  
 * Items used in this robot are as follows
 * - Lynxmotion Platform
 * - Arduino Mega
 * - Sabertooth Motor Controller
 * - HC-SR04 Ultrasonic Sensor
 * - RioRand 20x4 LCD Panel
 * - 4x3 Matrix Keypad
 * - 2 Miniature Breadboards
 * - SunFounder Active Buzzer
 * - Adafruit NeoPixel Ring (16-leds)
 * - Assorted wires
 * - 10k Potentiometer
 * - 220 ohm resistor
 */


// Include all necessary libraries
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>

// Define various pin constants;
const int sensor_Pin = 2;
const int leftMotorPin = 5;
const int rightMotorPin = 6;
const int alarmPin = 32;

// Declare random numbers
int rand1 = 0;
int rand2 = 0;

// Declare state-machine booleans
boolean isSolved = false;
boolean eButtonIsPressed = false;

// Declare integer arrays to hold values for current time and alarm time
int curTime[] = {0,0,0};
int alarmTime[] = {0,0,0};

// Declare servos for drivetrain
Servo leftMotor;
Servo rightMotor;

// Define necessary information used for the keypad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {42, 52 , 50, 46}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {44, 40, 48}; //connect to the column pinouts of the keypad

// Define Keypad using keypad library
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Define NeoPixel ring using NeoPixel library
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 24, NEO_GRB + NEO_KHZ800);

// Define and declare an LCD display
const int rs = 13, en = 12, d4 = 14, d5 = 15, d6 = 16, d7 = 17;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Set-up is the initial code the Arduino will run. This method runs once,
// and is then directed into the loop() function
void setup() {

  // Start the LCD panel
  lcd.begin(20, 4);
  lcd.setCursor(5,0);
  lcd.print("Booting up!");
  delay(2000);  
  
  //Serial.begin(9600);
  //Serial.println("Booting Up");

  // Create random numbers for arithmetic problem
  randomSeed(analogRead(1));
  
  // Create 2 random numbers between 50 and 99
  rand1 = random(50,99);
  rand2 = random(50,99);
  
  // Initialize neopixel ring
  strip.begin();
  strip.setBrightness(30); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'
  
  
  //Define digital pin modes
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

  //Set up Servo Outputs on pins defined above, initialize microstep
  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin); 

  //Serial.println("Enter Current Time - hhmmss:\n");
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Enter Current Time");  
  
  // Perform startClock() method to assign the current time
  startClock(curTime);

  // Initialize interrupt timer library for 1 million - milliseconds (1 second).
  // This allows the curTime array to stay in sync, even while everything else is going on.
  Timer1.initialize(1000000); 
  Timer1.attachInterrupt(timerInterrupt);

  // Prepare to write to the LCD
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Enter Alarm Time");
  //Serial.println("Enter Alarm Time");
  startClock(alarmTime);
}

// This method runs indefinitely, so long as power is provided to the Arduino
void loop() {
  
  // Print the time on the LCD panel
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("Time is:");
  lcd.setCursor(6,1);
  lcd.print(curTime[0]);
  lcd.print(":");
  lcd.print(curTime[1]);
  lcd.print(":");
  lcd.print(curTime[2]);

  // Print the alarm time on the LCD panel, under the current time
  lcd.setCursor(3,2);
  lcd.print("Alarm set for:");
  lcd.setCursor(6,3);
  lcd.print(alarmTime[0]);
  lcd.print(":");
  lcd.print(alarmTime[1]);
  lcd.print(":");
  lcd.print(alarmTime[2]);

  delay(100);

  // if current time equals alarm time, start alarm sequence
  if(curTime[0] == alarmTime[0] && curTime[1] == alarmTime[1] && curTime[2] == alarmTime[2]){
    // Begin driving
    drivetrainDrive(110);

    // Begin sounding the alarm tone on the buzzer
    pinMode(alarmPin, OUTPUT);
    digitalWrite(alarmPin, LOW);
    delay(1000);

    // While the math problem is not solved, drive around and be annoying
    while(!isSolved){

      // Perform a color wipe on the NeoPixel ring to turn it red
      colorWipe(strip.Color(255, 0, 0), 50); // Red

      // If the emergency button has not yet been pressed, ping the ultrasonic sensor 
      // to check if something is in the way.
      if(!eButtonIsPressed){
        
        // Save returned integer from the ping method
        int usDuration = ultrasonicPing(sensor_Pin);
  
        //Serial.print("Ulrasonic Sensor: ");
        //Serial.println(usDuration);

        // if the ultrasonic duration is less than 2000 microSeconds, 
        // turn the Lynxmotion Platform and resume driving forward
        if(usDuration < 2000){
          drivetrainTurn90();
          drivetrainDrive(110);
        } // end if
     }// end while

     // perform a color wipe on the NeoPixel ring to turn it off
     colorWipe(strip.Color(0, 0, 0), 50); // off 

     // clear the LCD panel and display the math problem
     lcd.clear();
     solveMath(rand1,rand2);
    
    }// end while
    
    // Once the problem is solved, turn off the drivetrain and alarm buzzer
    drivetrainStop();
    digitalWrite(alarmPin, HIGH);
    delay(1000);
  }// end if

}// end function

//**************************************************************************
// These functions provide basic code blocks to run the drivetrain

// Set both motors to the passed speed value
void drivetrainDrive(int speed){
  leftMotor.write(speed);
  rightMotor.write(speed);
}

// Turn both motors off
void drivetrainStop(){
  leftMotor.write(90);
  rightMotor.write(90);
}

// Turn 90 degrees to the right by making the left motor go forward
// and the right motor go backward
void drivetrainTurn90(){
  leftMotor.write(150);
  rightMotor.write(30);
  delay(500);
}

//**************************************************************************
// This function provides the functionality to use the ultrasonic sensor
// and return the duration in microSeconds back to the main loop

int ultrasonicPing(int signalPin){
    int duration = 0;
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
    duration = (int) pulseIn(signalPin, HIGH);

    return duration;
}

//**************************************************************************
// This function assigns the time to the two time arrays from a keypad

void startClock(int clockTime[]){  
  // Serial.println("In startClock");
  
  // Declare a character array of 0s to assign user input
  char thisTime[] = {'0','0','0','0','0','0'};

  // iterate 6 times, 1 for every digit required to define the time
  for(int i = 0; i < 6; i++){
    lcd.setCursor(6+i,1);
    //Serial.println("In For Loop");
    
    // create char to hold user input
    char key = keypad.getKey();

    // while user input equals nothing, keep polling
    
    // if in 10s column of hours, ignore 3-9,*,#
    if(i == 0){

      // The while loop forces the program to continue doing whatever its doing 
      // until the user enteres a value on the keypad. 
      while(key == NO_KEY || key == '*' || key == '#' 
          ||key == '3' || key == '4' || key == '5'|| key == '6'
          || key == '7'|| key == '8'|| key == '9' ){
        
        key = keypad.getKey();
      }// end while
    }// end if

    // if in 1s column of hours
    else if(i == 1){
      
      // if 10s column of hours == 2, ignore 4-9,*,#
      if(thisTime[0] == '2'){
         while(key == NO_KEY || key == '*' || key == '#' 
          ||key == '4' ||key == '5' ||key == '6' || key == '7'
          ||key == '8'|| key == '9' ){
        
            key = keypad.getKey();
          }// end while
      }// end if

      // else ignore *,#
      else{
        while(key == NO_KEY || key == '*' || key == '#'){
          key = keypad.getKey();
        }// end while
      }// end else
    }// end else if

    // if in 10s column of minutes, ignore 6-9,*,#
    else if(i == 2){
      while(key == NO_KEY || key == '*' || key == '#' 
          ||key == '6' || key == '7'|| key == '8'|| key == '9' ){
        
        key = keypad.getKey();
      }// end while
    }// end else if

    // if in 10s column of seconds, ignore 6-9,*,#
    else if(i == 4){
      while(key == NO_KEY || key == '*' || key == '#' 
          ||key == '6'|| key == '7'|| key == '8'|| key == '9' ){
        
        key = keypad.getKey();
      }// end while
    }// end else if    

    // else if i != 0,1,2,4, ignore *,#
    else{
      while(key == NO_KEY || key == '*' || key == '#'){
        key = keypad.getKey();
      }// end while
    }// end else
    
    // now that we have a value, save value and print
    thisTime[i] = key;
    lcd.print(key);
    Serial.print(key);
  }// end for loop
  
  // Concatenate digits, convert chars to Strings, convert Strings to Integers and save to array
  clockTime[0] = ((String)thisTime[0] + thisTime[1]).toInt();
  clockTime[1] = ((String)thisTime[2] + thisTime[3]).toInt();  
  clockTime[2] = ((String)thisTime[4] + thisTime[5]).toInt();

  // Clear the LCD
  lcd.clear();

  // Display confirmation message on LCD
  lcd.setCursor(4,0);
  lcd.print("You Entered: ");
  lcd.setCursor(6,1);
  lcd.print(clockTime[0]);
  lcd.print(":");
  lcd.print(clockTime[1]);
  lcd.print(":");
  lcd.print(clockTime[2]);

  //Serial.println();
  //Serial.print(clockTime[0]);
  //Serial.print(":");
  //Serial.print(clockTime[1]);
  //Serial.print(":");
  //Serial.print(clockTime[2]);

  // Perform a green color wipe on the NeoPixel ring as a positive confirmation
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 0), 50); // Off    

  delay(1000);  
}

// This function takes a clock array and check to ensure that
// the hours, minutes, and seconds can roll over if needed
void incrementClock(int clockTime[]){
  clockTime[2]++;

  // if seconds > 59, increment minutes, reset seconds
  if(clockTime[2] > 59){
    clockTime[1]++;
    clockTime[2] = 0;
  }

  // if minutes > 59, increment hours, reset minutes
  if(clockTime[1] > 59){
    clockTime[0]++;
    clockTime[1] = 0;
  }

  // if hours > 23, reset hours
  if(clockTime[0] > 23){
    clockTime[0] = 0;
  }
}

//**************************************************************************
// This function takes two integers to display on the screen and waits for user 
// input to solve basic arithmetic problem.


void solveMath(int rand1, int rand2){
  // create user integer to compare against actual
  int userSol = 0;

  // while userSoluition does not equal the sum of the two random numbers...
  while(userSol != rand1+rand2){
    
    // create char array to hold individual user inputs 
    char userArray[] = {0,0,0};

    // give prompt to user
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Solve this problem");
    lcd.setCursor(7,1);
    lcd.print(rand1);
    lcd.print("+");
    lcd.print(rand2);

    
    //Serial.print("Solve: ");
    //Serial.print(rand1);
    //Serial.print(" + ");
    //Serial.print(rand2);
    //Serial.println(", add leading 0s");
    

    // Iterate through userArray, waiting for user input to fill in elements
    for(int i = 0; i < 3; i++){
  
      //Serial.println("In Math For Loop");
      
      // Create char to hold user input
      char key = keypad.getKey();
  
      // While user input equals nothing, keep polling
      while(key == NO_KEY){
        //Serial.println("waiting ");
        key = keypad.getKey();

        // Use the # or * as emergency wheel stops 
        if(key == '*' || key == '#'){
          // Turn off wheels
          drivetrainStop();
          // Set boolean to true
          eButtonIsPressed = true;

          // Display on LCD confirmation message
          lcd.setCursor(7,4);
          lcd.print("Stopped");
          
          // Decrement i interator value. This allows the * or # to be 
          // pressed without affecting the for loop to solve the math problem
          i--;
        }

        delay(100);
        
        // If the emergency button has not been pressed, ping 
        // ultrasonic sensors to check if something is in the way.
        if(!eButtonIsPressed){
          int usDuration = ultrasonicPing(sensor_Pin);
    
          //Serial.print("Ulrasonic Sensor: ");
          //Serial.println(usDuration);
          if(usDuration < 2000){
            drivetrainTurn90();
            drivetrainDrive(110);
          }// end if
        }// end if
      }// end while
  
      // now that we have a value, check that it is not a # or *, 
      // and then save value and print to LCD
      if(key != '*' && key != '#'){
        lcd.setCursor(8+i,2);
        lcd.print(key);
        //Serial.print(key); 
        userArray[i] = key;
      }
    }

    // Concatenate individual char keys into String, then convert to Int
    userSol = ((String)userArray[0] + userArray[1] + userArray[2]).toInt();

    // If the user solution is not the correct answer, do a red color wipe 
    // on the NeoPixel to indicate an incorrect answer
    if(userSol != rand1+rand2){
      colorWipe(strip.Color(255, 0, 0), 50); // Red
      colorWipe(strip.Color(0, 0, 0), 50); // Off
    }// end if
  }// end while

  // If the user solution is correct, do a green color wipe on the
  // NeoPixel to indicate a correct answer
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 0), 50); // Off
  
  // Displayy confirmation message on the LCD
  //Serial.println("\nSolved!");
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("SOLVED!");
  delay(1000);

  // Set the isSolved boolean to true to exit the main while loop in loop() method
  isSolved = true;
}

//**************************************************************************
// This function provides the capability to use the NeoPixel ring as a color wipe

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//**************************************************************************
// This function is the Interrupt Service Routine for the timer. Every second, increment the current time
void timerInterrupt(){
  incrementClock(curTime);
}


