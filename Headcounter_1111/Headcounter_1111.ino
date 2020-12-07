/* 
Covid Robot Project 
Members: Ryan Lake, Coby Hirsh, Tyler Krasignor, Myles Karna, Ankit
lake.ry@northeastern.edu
Displays the number of people in a building sensing who enters and exits the building. The distance
at which the device is setup between walls can be pre-setup with the two buttons allowing for varying
distances at which the device will recognize a person walking by. 
*/


#include <LiquidCrystal.h>         // the liquid crystal library contains commands for printing to the display

const int trigPin_1 = 5;           // connects to the trigger and echo pin on distance sensor 1/2
const int echoPin_1 = 6;           
const int trigPin_2 = 10;          
const int echoPin_2 = 11;          

int redKeyPin = 9;                // pin for the red/green button
int greenKeyPin = 8;             

LiquidCrystal lcd(13, 12, 7, 4, 3, 2);   // tell the RedBoard what pins are connected to the display
   
bool sensorDetect1 = false;              // stores whether sensor sees a person or not     
bool sensorDetect2 = false;  

void setup() {
  Serial.begin (9600);        //set up a serial connection with the computer

  pinMode(trigPin_1, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(echoPin_1, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor
  pinMode(trigPin_2, OUTPUT);   
  pinMode(echoPin_2, INPUT);
  
  pinMode(redKeyPin, INPUT_PULLUP);     // the setup for buttons to be pullup resistors
  pinMode(greenKeyPin, INPUT_PULLUP);

  lcd.begin(16, 2);                 //lcd display of 16 characters wide and 2 characters high
  lcd.clear();                     
  
}

void loop() {

  // variables:
  bool loop = true;
  int numPeople = 0;
  int numPeopleIn = 0;
  int numPeopleOut = 0;
  int hallwayL = 0;
  float distance1 = 0;
  float distance2 = 0;
  
    
  //output the distance every time interval
  while (loop == true) {
    // Button code for how long the sensors have until a wall
    if (digitalRead(greenKeyPin) == LOW) {
      hallwayL = 10;
    }
    if (digitalRead(redKeyPin) == LOW) {
      hallwayL = 20;
    }

    // to hold the distance both sensors read
    distance1 = getDistance(trigPin_1, echoPin_1);
    distance2 = getDistance(trigPin_2, echoPin_2);
    
    // main logic of the code:
    numPeopleIn = checkSensor1(hallwayL, distance1);
    numPeopleOut = checkSensor2(hallwayL, distance2);
    numPeople = numPeopleIn - numPeopleOut;
        
    lcd.setCursor(0, 0);
    lcd.print("People: ");
    lcd.print(numPeople);
    delay(500);
  }
}

//FUNCTIONS:

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance(int trigPin, int echoPin) {
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}

// returns the number of people entering the building from sensor 1
int checkSensor1 (int sensorDistance, float distanceValue) {
  int numPeopleEnter = 0;
  if (distanceValue < sensorDistance) { 
    if (sensorDetect1 == false) {
      numPeopleEnter += 1;
      sensorDetect1 = true;
    }
  }
  if (distanceValue > sensorDistance) {
    sensorDetect1 = false;
  }
  return numPeopleEnter;
}


// returns the number of people exiting the building from sensor 2
int checkSensor2 (int sensorDistance, float distanceValue) {
  int numPeopleExit = 0;
  if (distanceValue < sensorDistance) {  
    if (sensorDetect2 == false) {
      numPeopleExit -= 1;
      sensorDetect2 = true;
    }
  }
    if (distanceValue > sensorDistance) {
      sensorDetect2 = false;
    }
  return numPeopleExit;
}