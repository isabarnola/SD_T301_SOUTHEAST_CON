#include <SoftwareSerial.h>
#include "TimerOne.h"
#include <QMC5883L.h>
#include <Wire.h>
#include <Servo.h>
#include <NewPing.h>        //Ultrasonic sensor function library. You must install this library

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
int ultraSonic_distance = 100;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
Servo servo_motor; //our servo name
int distanceRight = 0;
int distanceLeft = 0;
// https://www.instructables.com/id/Arduino-Robot-With-Distance-Direction-and-Degree-o/

QMC5883L compass; // For GY-271 
int heading; // Input from Sensor  GY-271
Servo servo_1; // Servo motor
String input = ""; // Variable to store Voice Input
const int numberOfwords = 5; // Max no of words in the Voice input, Will be used as a array length

String words[numberOfwords]; // Arrary to store individual words from voice input

SoftwareSerial BlueT(0, 1); //TX, RX connections 

const byte MOTOR_A = 2;  // Encoder Motor Left Channel A 
const byte MOTOR_B = 3;  // Encoder Motor Right Channel A 
const float stepcount = 520;  // No of pulses per complete rotation of motor shaft
const float wheeldiameter = 70; // Wheel diameter in millimeters, change if different

volatile int counter_A = 0;
volatile int counter_B = 0;
int Distance ;
String Direction = "";
int angle;
int angleflag = 0;
String auto_mode ="OFF";

// Motor A
int in1 = 6;
int in2 = 7;

// Motor B
int in3 = 8;
int in4 = 9;
int counter = 0;

int lastIndex = 0;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
   BlueT.begin(9600); 
     // Attach the Interrupts to their ISR's
  attachInterrupt(digitalPinToInterrupt (MOTOR_A), ISR_countA, RISING);  // Increase counter A when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR_B), ISR_countB, RISING);  // Increase counter B when speed sensor pin goes High
  compass.init();
  compass.setSamplingRate(50);
  servo_motor.attach(10); //our servo pin

  servo_motor.write(90);
  delay(1000);
  ultraSonic_distance = readPing();
  Serial.println("Setup");
Serial.print("Ping: ");
  Serial.print(ultraSonic_distance); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");
}


void loop() 
{
   heading = compass.readHeading();
   if(heading==0) {
    /* Still calibrating, so measure but don't print */
  } else {
    Serial.println(heading);
  }


// Accept Voice Input and store it into variable input

    while (BlueT.available()){  //Check if there is an available byte to read
  delay(10); //Delay added to make thing stable
  char c = BlueT.read(); //Conduct a serial read 
  input += c; //build the string- "forward", "reverse", "left" and "right" 
  }
input = input.substring(0, input.length() - 1); //  remove last character "*" from the string 
Serial.println(input);

// Split the input sentence into words and store them in arrary
      for (int i = 0; i < input.length(); i++) {
        // Loop through each character and check for space between inputs
        if (input.substring(i, i+1) == " ") {
          // Grab the input from the last index up to the current position and store it
          words[counter] = input.substring(lastIndex, i);
          // Update the last position and add 1, so it starts from the next character
          lastIndex = i + 1;
          // Increase the position in the array that we store into
          counter++;
        }

        // If we're at the end of the string 
        if (i == input.length() - 1) {
          // Grab the last part of the string from the lastIndex to the end
          words[counter] = input.substring(lastIndex, input.length());
        }
      }
     
      // Clear out string and counters to get ready for the next incoming string
      input = "";
      counter = 0;
      lastIndex = 0;
Serial.println(words[0]);
Serial.println(words[1]);
Serial.println(words[2]);
Serial.println(words[3]);
Serial.println(words[4]);
Serial.println(words[5]);



if ((words[0] == "*forward") || (words[0] == "*reverse") || (words[0] == "*left") || (words[0] == "*right") || (words[0]=="*auto") || (words[0]=="*line") || (words[0]=="*stop`"))
{
  Direction = words[0];
  if (words[1] == NULL)
  {
    Distance = 0;
    angleflag = 0;
  }
  if  (words[1]== "distance")
  {
    Distance = words[2].toInt(); 
  }
  if (words[1] == "angle")
  {
    angle = words[2].toInt();
    angleflag = 1;
  }

  if (words[3] =="distance")
  {
    Distance = words[4].toInt(); 
  }
  if (words[3]== "angle")
  {
    angle = words[4].toInt();
    angleflag = 1;
  } 
}

if (words[0]== "*angle")
{
 angle =  words[1].toInt();
 angleflag = 1;
if (angleflag == 1)
{ 
  Oritntation(angle);
}
}

if (words[0]== "*auto")
{
auto_mode = "ON"; 
  Auto_mode();
}


if (Direction == "*forward")
{
  if (angleflag == 1)
  {  
  Oritntation(angle);
  }
if (Distance == 0)
  {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  }
  else 
  {
    MoveForward(CMtoSteps(Distance));    
  }
}
  else if (Direction == "*reverse")
{
  if (angleflag == 1)
  {
  Oritntation(angle);
  }
if (Distance == 0)
  {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  }
  else 
  {
    MoveReverse(CMtoSteps(Distance));
  }
  }
  else if (Direction == "*stop")
  {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  }
  else if (Direction == "*left")
  {
  if (angleflag == 1)
  {
  Oritntation(angle);
  }
  if (Distance == 0)
  {
  SpinLeft(CMtoSteps(19)); 
  }
  else 
  {
  SpinLeft(CMtoSteps(Distance)); 
  }
  }
  
  else if (Direction == "*right")
  {
  if (angleflag == 1)
  {
  Oritntation(angle);
  }
  if (Distance == 0)
  {
  SpinRight(CMtoSteps(19)); 
  }
  else 
  {
  SpinRight(CMtoSteps(Distance));
  }
  }  
  input="";
  Distance = NULL;
  Direction = "";
  angleflag = 0;
  words[0] = "";
  words[1] = "";
  words[2] = "";
  words[3] = "";
  words[4] = "";
  }

// Interrupt Service Routines

// Motor A pulse count ISR
void ISR_countA()  
{
  counter_A++;  // increment Motor A counter value
} 

// Motor B pulse count ISR
void ISR_countB()  
{
  counter_B++;  // increment Motor B counter value
}


// Function to convert from centimeters to steps
int CMtoSteps(float cm) 
{
  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step
  float f_result = cm / cm_step;  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)  
  return result;  // End and return result
}

// Function to Move Forward
void MoveForward(int steps) 
{
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   while (steps > counter_A || steps > counter_B) {
   // Set Motor A forward
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);

   // Set Motor B forward
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   }
  // Stop when done
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);

   // Set Motor B forward
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero 
  }

// Function to Move in Reverse
void MoveReverse(int steps) 
  {
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   while (steps > counter_A || steps > counter_B) {
   // Set Motor A reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
   }
     
  // Stop when done
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 

}

// Function to Spin Right
void SpinRight(int steps) 
{
  Serial.println("Entering Right Function");
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   while (steps > counter_A || steps > counter_B){
   // Set Motor A reverse
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Set Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
   }
   
   
    
  // Stop when done
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 

}

// Function to Spin Left
void SpinLeft(int steps) 
{
   counter_A = 0;  //  reset counter A to zero
   counter_B = 0;  //  reset counter B to zero
   while (steps > counter_A || steps > counter_B) { 
   // Set Motor A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
   }
      
  // Stop when done
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  counter_A = 0;  //  reset counter A to zero
  counter_B = 0;  //  reset counter B to zero 
}

// Move the robot by desired angle with 0 degree as North

void Oritntation(int Angle_1)

{
int Difference = Angle_1 - heading; // if difference is negative move left or else move right
int Tolerance1 = 1; 
int Tolerance2 = -1;
while (Difference < Tolerance2 || Difference > Tolerance1)
{
  angleflag = 0;
  heading = compass.readHeading();
  Difference = Angle_1 - heading;
 if ((Difference >= 90 && Difference < 180) || (Difference <= -90 && Difference > -180)){
 SpinRight(20);
 heading = compass.readHeading();
 //delay(1000);
 }
 
 if ((Difference >= 180 && Difference < 270) || (Difference <= -180 && Difference > -270)){
 SpinRight(40);
 heading = compass.readHeading();
 //delay(1000);
 }
 
 if ((Difference >= 270 && Difference < 360) || (Difference <= -270 && Difference > -360)){
 SpinRight(60);
 heading = compass.readHeading();
 //delay(1000);
 }

if ((Difference >= 45 && Difference < 90) || (Difference <= -45 && Difference > -90)){
 SpinRight(10);
 heading = compass.readHeading();
 //delay(1000);
 }

 if ((Difference >= 10 && Difference < 45) || (Difference <= -10 && Difference > -45)){
 SpinRight(5);
 heading = compass.readHeading();
 //delay(1000);
 } 

if ((Difference >= 3 && Difference < 10) || (Difference <= -3 && Difference > -10)){
 SpinRight(1);
 heading = compass.readHeading();
 //delay(1000);
 } 
 
 
}
}

void Auto_mode(){
  while (auto_mode == "ON"){
    ultraSonic_distance = readPing();
    if (ultraSonic_distance <= 20){
  moveStop_Auto();
    delay(100);
    moveBackward_Auto();
    delay(200);
    moveStop_Auto();
    delay(100);
    
  distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);
  if (ultraSonic_distance >= distanceLeft){
  Serial.println("Turning RIGHT");
      turnRight_Auto();
       delay(500);
      moveStop_Auto();
    }
    else{
      Serial.println("Turning Left");
      turnLeft_Auto();
        delay(500);
      moveStop_Auto();
    }
  }
  else{
    moveForward_Auto(); 
  }
    ultraSonic_distance = readPing();
}

    }

int lookRight(){  
  servo_motor.write(45);
  delay(500);
  int ultraSonic_distance = readPing();
  delay(100);
  servo_motor.write(90);
  return ultraSonic_distance;
}

int lookLeft(){
  servo_motor.write(160);
  delay(500);
  int ultraSonic_distance = readPing();
  delay(100);
  servo_motor.write(115);
  return ultraSonic_distance;
  delay(100);
}

void moveStop_Auto(){

digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  
}

void moveBackward_Auto()
{
digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
} 

void turnRight_Auto()
{
  Serial.println("Motor Right");
digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  // Set Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}  

void turnLeft_Auto()
{
Serial.println("Motor Left");
digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  // Set Motor B reverse
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}  

void moveForward_Auto()
{
 digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);

   // Set Motor B forward
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
} 

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}