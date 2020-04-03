#include <Stepper.h>

//this code was tested at 6V

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor
int steps = 0;  // steps to be used in function to convert distance to number of steps


// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 11, 13);

//This function converts the block height to a number of steps for the stepper to take.
int d2s(float block, int dir){
//  block is the desired travel distance for the claw
//  dir is the desired direction of travel; 1 to move up and -1 to move down.
  steps = -dir*block*19.2*200/8;
  
  return steps;
}

//This function converts a desired velocity in cm/s to the required rpm
int Speed(int vel){
  int rpm = vel*10*60/8;
  
  return rpm;
}


void setup() {
  // set the speed at desired rpm:
  myStepper.setSpeed(275);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
 
  myStepper.step(d2s(4,1));
  delay(10000000);
}
