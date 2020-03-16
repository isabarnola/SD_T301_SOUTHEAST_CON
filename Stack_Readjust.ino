#include <Stepper.h>
#include<Servo.h>


const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor
int steps = 0;  // steps to be used in function to convert distance to number of steps
int counter = 0; // Block height of upper claw
int pos = 1;
#define UP 1
#define DOWN -1
#define CLOSE 150     //Arrange the top close value between 145 and 150
#define BCLOSE 55     //Arrange between 50 and 55 for tightness of the bottom claw
#define OPEN 180
// servoa <= Top Claw
// servob <= Bottom Claw
Servo top, bottom;         
Stepper myStepper(stepsPerRevolution, 30, 31, 32, 33);
#define stepp 2
int r_stepper = 2;
int l_stepper = 3;

int action = 1;
int d2s(float block, int dir){
//  block is the desired travel distance for the claw
//  dir is the desired diretion of travel; 1 to move up and -1 to move down.
  analogWrite(r_stepper, 200);
  analogWrite(l_stepper, 200);
  steps = dir*block*19.2*200/8;
  
  return steps;
} 
void first_clamp () {         //4 blocks
  analogWrite(r_stepper, 200);
  analogWrite(l_stepper, 200);
  bottom.write(BCLOSE);
  delay(500);
  top.write(CLOSE);
  delay(1000);
  bottom.write(OPEN);
  delay(500);     //Reduce to as low as 250.
  myStepper.step(d2s(3, UP));
  analogWrite(r_stepper, 0);
  analogWrite(l_stepper, 0);
  pos++;
}
void clamp () {
  analogWrite(r_stepper, 200);
  analogWrite(l_stepper, 200);
  bottom.write(BCLOSE);
  delay(100);
  myStepper.step(d2s(stepp, DOWN));
  delay(500);
  top.write(OPEN);
  delay(500);
  myStepper.step(d2s(1, DOWN));
  delay(500);
  top.write(CLOSE);
  delay(500);
  bottom.write(OPEN);
  delay(250);
  myStepper.step(d2s(stepp+pos, UP));
  analogWrite(r_stepper, 0);
  analogWrite(l_stepper, 0);
  pos ++;
    }
void setup() {
  // put your setup code here, to run once:
top.attach(9);
bottom.attach(11);
myStepper.setSpeed(200);
top.write(OPEN);
bottom.write(OPEN);

delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
if (action == 1){
  first_clamp();
  clamp();
  clamp();
  action = 2;
  }
  else;
  analogWrite(r_stepper, 0);
  analogWrite(l_stepper, 0);
//  servob.write(0);
//  delay(1000);
//  servoa.write(100);
//  delay(3000);
//  servob.write(180);
//  delay(500);
// myStepper.step(d2s(2, UP));
// delay(10000);
//delay(1000000);

 
}
