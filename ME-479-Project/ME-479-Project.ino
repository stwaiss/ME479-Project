// Include all necessary libraries
#include <Servo.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

// Define various pin constants;
const int eButtonPin = 100;
const int sensor_Pin = 2;
const int leftMotorPin = 5;
const int rightMotorPin = 6;

int curTime[] = {0,0,0};
int alarmTime[] = {0,0,0};

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

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void setup() {
  Serial.begin(9600);
  Serial.println("Booting Up");
  
  //Define digital pin modes
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

  leftMotor.attach(leftMotorPin);                       //Set up Servo Outputs on pins defined above, initialize microstep
  rightMotor.attach(rightMotorPin); 

  Serial.println("Enter Current Time - hhmmss:\n");
  startClock(curTime);
  startClock(alarmTime);
  
}

void loop() {
//  drivetrainDrive(125);
//
//// Ping ultrasonic sensors to check if something is in the way.
//   int usDuration = ultrasonicPing(sensor_Pin);
//
//   Serial.print("Ulrasonic Sensor: ");
//   Serial.println(usDuration);
//   if(usDuration < 2000){
//      drivetrainTurn90();
//      drivetrainDrive(125);
//   }
//
//  delay(100);
       
}

//****************************************************************************************

void drivetrainDrive(int speed){
  leftMotor.write(speed);
  rightMotor.write(speed);
}

void drivetrainStop(){
  leftMotor.write(90);
  rightMotor.write(90);
}

void drivetrainTurn90(){
  leftMotor.write(150);
  rightMotor.write(30);
  delay(500);
}

//*************************************************************************
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
void startClock(int clockTime[]){  
  Serial.println("In startClock");
  char thisTime[] = {0,0,0,0,0,0};

  // iterate 6 times, 1 for every digit required to define the time
  for(int i = 0; i < 6; i++){

    Serial.println("In For Loop");
    
    // create char to hold user input
    char key = keypad.getKey();

    // while user input equals nothing, keep polling
    while(key == NO_KEY){
      //Serial.println("waiting ");
      key = keypad.getKey();
    }

    // now that we have a value, save value and print
    thisTime[i] = key;
    Serial.print(key);
  }

  // Concatenate digits, convert to integer, and save to array
  clockTime[0] = ((String)thisTime[0] + thisTime[1]).toInt();
  clockTime[1] = ((String)thisTime[2] + thisTime[3]).toInt();  
  clockTime[2] = ((String)thisTime[4] + thisTime[5]).toInt();

  Serial.println();
  Serial.print(clockTime[0]);
  Serial.print(":");
  Serial.print(clockTime[1]);
  Serial.print(":");
  Serial.print(clockTime[2]);      
}





