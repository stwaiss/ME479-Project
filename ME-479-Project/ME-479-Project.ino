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

int rand1 = 0;
int rand2 = 0;
boolean isSolved = false;
boolean eButtonIsPressed = false;
boolean alarmIsSounding = false;

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
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, 24, NEO_GRB + NEO_KHZ800);

const int rs = 13, en = 12, d4 = 14, d5 = 15, d6 = 16, d7 = 17;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(20, 4);
  lcd.setCursor(5,0);
  lcd.print("Booting up!");
  delay(2000);  
  
  Serial.begin(9600);
  Serial.println("Booting Up");

  // create random numbers for arithmetic problem
  randomSeed(analogRead(1));
  rand1 = random(50,99);
  rand2 = random(50,99);
  
  // initialize neopixel ring
  strip.begin();
  strip.setBrightness(30); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'
  
  
  //Define digital pin modes
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);

  leftMotor.attach(leftMotorPin);                       //Set up Servo Outputs on pins defined above, initialize microstep
  rightMotor.attach(rightMotorPin); 

  //Serial.println("Enter Current Time - hhmmss:\n");
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Enter Current Time");  
  startClock(curTime);

  // Initialize interrupt timer library for 1 million - milliseconds (1 second);
  Timer1.initialize(1000000); 
  Timer1.attachInterrupt(timerInterrupt);

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Enter Alarm Time");
  //Serial.println("Enter Alarm Time");
  startClock(alarmTime);
}

void loop() {

  //print the time on the LCD panel)
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("Time is:");
  lcd.setCursor(6,1);
  lcd.print(curTime[0]);
  lcd.print(":");
  lcd.print(curTime[1]);
  lcd.print(":");
  lcd.print(curTime[2]);

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
    // sound alarm
    // turn on light ring
    // start driving
    // display math problem to be solved
    
    drivetrainDrive(110);

    pinMode(alarmPin, OUTPUT);
    digitalWrite(alarmPin, LOW);
    delay(1000);

    // while math problem is not solved, drive around and be annoying
    while(!isSolved){
      alarmIsSounding = true;
      colorWipe(strip.Color(255, 0, 0), 50); // Red

      // Ping ultrasonic sensors to check if something is in the way.
      if(!eButtonIsPressed){
        int usDuration = ultrasonicPing(sensor_Pin);
  
        Serial.print("Ulrasonic Sensor: ");
        Serial.println(usDuration);
        if(usDuration < 2000){
          drivetrainTurn90();
          drivetrainDrive(110);
        }
     }
     colorWipe(strip.Color(0, 0, 0), 50); // off 

     lcd.clear();
     solveMath(rand1,rand2);
    }
    drivetrainStop();
    digitalWrite(alarmPin, HIGH);
    delay(1000);
  }

}

//**************************************************************************

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

//**************************************************************************
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
  char thisTime[] = {'0','0','0','0','0','0'};

  // iterate 6 times, 1 for every digit required to define the time
  for(int i = 0; i < 6; i++){
    lcd.setCursor(6+i,1);
    Serial.println("In For Loop");
    
    // create char to hold user input
    char key = keypad.getKey();

    // while user input equals nothing, keep polling
    
    // if in 10s column of hours, ignore 3-9,*,#
    if(i == 0){
      while(key == NO_KEY || key == '*' || key == '#' 
          ||key == '3' || key == '4' || key == '5'|| key == '6'
          || key == '7'|| key == '8'|| key == '9' ){
        
        key = keypad.getKey();
      }
    }

    // if in 1s column of hours
    else if(i == 1){
      
      // if 10s column of hours == 2, ignore 4-9,*,#
      if(thisTime[0] == '2'){
         while(key == NO_KEY || key == '*' || key == '#' 
          ||key == '4' ||key == '5' ||key == '6' || key == '7'
          ||key == '8'|| key == '9' ){
        
            key = keypad.getKey();
          }
      }

      // else ignore *,#
      else{
        while(key == NO_KEY || key == '*' || key == '#'){
          key = keypad.getKey();
        }
      }
    }

    // if in 10s column of minutes, ignore 6-9,*,#
    else if(i == 2){
      while(key == NO_KEY || key == '*' || key == '#' 
          ||key == '6' || key == '7'|| key == '8'|| key == '9' ){
        
        key = keypad.getKey();
      }
    }

    // if in 10s column of seconds, ignore 6-9,*,#
    else if(i == 4){
      while(key == NO_KEY || key == '*' || key == '#' 
          ||key == '6'|| key == '7'|| key == '8'|| key == '9' ){
        
        key = keypad.getKey();
      }
    }    

    // else if i != 0,1,2,4, ignore *,#
    else{
      while(key == NO_KEY || key == '*' || key == '#'){
        key = keypad.getKey();
      }
    }
    
    // now that we have a value, save value and print
    thisTime[i] = key;
    lcd.print(key);
    Serial.print(key);
  }
  
  // Concatenate digits, convert to integer, and save to array
  clockTime[0] = ((String)thisTime[0] + thisTime[1]).toInt();
  clockTime[1] = ((String)thisTime[2] + thisTime[3]).toInt();  
  clockTime[2] = ((String)thisTime[4] + thisTime[5]).toInt();

  lcd.clear();

  lcd.setCursor(4,0);
  lcd.print("You Entered: ");
  lcd.setCursor(6,1);
  lcd.print(clockTime[0]);
  lcd.print(":");
  lcd.print(clockTime[1]);
  lcd.print(":");
  lcd.print(clockTime[2]);

  Serial.println();
  Serial.print(clockTime[0]);
  Serial.print(":");
  Serial.print(clockTime[1]);
  Serial.print(":");
  Serial.print(clockTime[2]);

  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 0), 50); // Off    

  delay(1000);  
}

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
void solveMath(int rand1, int rand2){
  // create user integer to compare against actual
  int userSol = 0;

  // while userSoluition != the sum of the two random numbers...
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

    
    Serial.print("Solve: ");
    Serial.print(rand1);
    Serial.print(" + ");
    Serial.print(rand2);
    Serial.println(", add leading 0s");
    

    // iterate through userArray, waiting for user input to fill in elements
    for(int i = 0; i < 3; i++){
  
      //Serial.println("In Math For Loop");
      
      // create char to hold user input
      char key = keypad.getKey();
  
      // while user input equals nothing, keep polling
      while(key == NO_KEY){
        //Serial.println("waiting ");
        key = keypad.getKey();

        if(key == '*' || key == '#'){
          drivetrainStop();
          eButtonIsPressed = true;
          lcd.setCursor(7,4);
          lcd.print("button");
          i--;
        }

        delay(100);
        // Ping ultrasonic sensors to check if something is in the way.
        if(!eButtonIsPressed){
          int usDuration = ultrasonicPing(sensor_Pin);
    
          Serial.print("Ulrasonic Sensor: ");
          Serial.println(usDuration);
          if(usDuration < 2000){
            drivetrainTurn90();
            drivetrainDrive(110);
          }
        }
      }
  
      // now that we have a value, save value and print
      if(key != '*' && key != '#'){
        lcd.setCursor(8+i,2);
        lcd.print(key);
        Serial.print(key); 
        userArray[i] = key;
      }
    }

    // Concatenate individual char keys into String, then convert to Int
    userSol = ((String)userArray[0] + userArray[1] + userArray[2]).toInt();

    if(userSol != rand1+rand2){
      colorWipe(strip.Color(255, 0, 0), 50); // Red
      colorWipe(strip.Color(0, 0, 0), 50); // Off
    }
  }
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 0), 50); // Off
  Serial.println("\nSolved!");
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("SOLVED!");
  delay(1000);
  isSolved = true;
}

//**************************************************************************

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

//**************************************************************************
void timerInterrupt(){
  incrementClock(curTime);
}


