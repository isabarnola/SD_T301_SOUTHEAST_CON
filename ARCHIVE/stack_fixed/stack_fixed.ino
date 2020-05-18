#include<Servo.h>
#include <Stepper.h>
#include <AccelStepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor
Servo servo_top, servo_bot;
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);
int ir = 5;

//Servo position for each claw
int bopen = 180;
int bclose = 0;
int topen = 80;
int tclose = 118;

#define dirPin  4 // green wire attached to green
#define stepPin 5 // yellow wire attached to yellow
#define motorInterfaceType 1
#define speed_stepper 800
float total_steps = 0;
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);//

// direction of stepper
const int up = 1;
const int down = -1;

// Position of claw
float pos = 0;
// Stack height
float stack = 0;
float actualStack = 5;

int sm1 = 42;
int sm2 = 43;
int speedSM = 200; 

void total_s(int steps, int dir)
{
  if (dir == up)
  {
    total_steps = total_steps -(steps);
    }
    else 
    {
      total_steps = total_steps + -(steps);
      }
  }

void initialize()
{
    servo_bot.write(bopen);
    servo_top.write(90);
  }

void setup() {
  // put your setup code here, to run once:
servo_top.attach(9);
servo_bot.attach(8);
//myStepper.setSpeed(275);
stepper.setMaxSpeed(1500);
 stepper.setCurrentPosition(0);
 
Serial.begin(9600);
initialize();
}

//This function converts the block height to a number of steps for the stepper to take.
int d2s(float block, int dir)
{
//  block is the desired travel distance for the claw
//  dir is the desired direction of travel; 1 to move up and -1 to move down.
  int steps = 0;
  steps = -dir*block*19.2*200/8;
  pos += block*dir;
      //Serial.print(pos);
  return steps;
}


void m(int steps, int velocity)
{
   while (stepper.currentPosition() >= steps)
  {
    stepper.setSpeed(velocity);
    stepper.runSpeed();
  }
}

void moveStepper(int steps, int velocity, int dir)
{
 if (dir == up)
 {
    Serial.print("up");
  while (stepper.currentPosition() != steps)
  {
   // Serial.print("Current steps ");
  // Serial.println(-stepper.currentPosition());
    //Serial.print("\tSet to steps ");
    //Serial.println(steps);
    stepper.setSpeed(velocity);
    stepper.runSpeed();
    }
 }
  else
  {
   while (stepper.currentPosition() != steps)
   {
    //Serial.print("Current steps ");
    //Serial.println(stepper.currentPosition());
    //Serial.print("\tSet to steps ");
    //Serial.println(steps);
    stepper.setSpeed(-velocity);
    stepper.runSpeed();
    }
  }
}    

void firstStack()
{
  //digitalWrite(sm1, speedSM);
  //digitalWrite(sm2, speedSM);
  Serial.println("closing bottom claw");
  servo_bot.write(bclose);
  
  delay(1000);
  Serial.println("closing top claw");
  servo_top.write(tclose);
  
  delay(500);
  Serial.println("actively closing top claw");
  servo_top.write(90);
  
  Serial.println("opening bottom claw");
  servo_bot.write(bopen);
   delay(1000);
  
   Serial.println("going up by 3");
  //myStepper.step(d2s(3,up));
  //stepper.setSpeed(d2s(3,up));
  Serial.println(d2s(3,up) );
  //stepper.setSpeed(400);
 // stepper.runSpeed();
 //Serial.print((d2s(3,up));
  int t = d2s(3,up);
   total_s(t,up);
  moveStepper(total_steps,800,up);
  
  stack+=1;
  }

  void lastStack(){
     //digitalWrite(sm1, speedSM);
     //digitalWrite(sm2, speedSM);
    servo_bot.write(bclose);
    servo_top.write(tclose);
    delay(500);
    //myStepper.step(d2s(stack,down));
    /*
     stepper.setSpeed(d2s(stack,down));
     stepper.runSpeed();
     */
      int t = d2s(stack,down);
    total_s(t,down);
     moveStepper(d2s(stack,down),speed_stepper,down);
  }
  
void bstack()
{
  //digitalWrite(sm1, speedSM);
  //digitalWrite(sm2, speedSM);
  
  stack+=1;
  Serial.print("\n Stacking");
  
  //  Close bottom claw on block
  Serial.println(" 1 Close bottom claw on block");
  servo_bot.write(bclose);
  delay(1000);

  // Actively close on block
  Serial.print("\n 2 Closing Top");
  servo_top.write(tclose);
  delay(500);

  // Lower Stepper to place stack on block
  Serial.print("\n 3  Moving Down");
  //myStepper.step(d2s(pos-stack+1.4,down));
  //stepper.setSpeed(d2s(pos-stack+1.4,down));
  //stepper.runSpeed();
    int t = d2s(stack +0.7,down);
    total_s(t,down);
   moveStepper(total_steps,speed_stepper,down);
  delay(1500);

/*
  //open top claw
  Serial.print("\n 4 Opening Claw");
  servo_top.write(topen);
  delay(1500);
  servo_top.write(90);
  delay(500);

  // Move top Claw above stack
  Serial.print("\n 5 Moving top claw Up");
  //myStepper.step(d2s(stack+1-pos,up));
  //stepper.setSpeed(d2s(stack+1-pos,up));
  //stepper.runSpeed();
  t = d2s(stack+1,up);
    total_s(t,up);
  moveStepper(total_steps,speed_stepper,up);
  
  delay(1500);
  
  // Close top claw
  Serial.print("\n 6 Closing top Claw");
  servo_top.write(tclose);
  delay(500);
  servo_top.write(90);
  
// push down stack from top
Serial.print("\n 7 Moving top claw Down");
  //myStepper.step(d2s(pos-stack,down));
   //stepper.setSpeed(d2s(pos-stack,down));
  //stepper.runSpeed();
  t = d2s(stack-1,down);
   total_s(t,down);
   moveStepper(total_steps,speed_stepper,down);
  
  delay(1500);
  Serial.println("8 Move up again ");
  //myStepper.step(d2s(stack/2,up));
  //stepper.setSpeed(d2s(stack/2,up));
  //stepper.runSpeed();
   t = d2s(stack/2,up);
   total_s(t,up);
  moveStepper(total_steps,speed_stepper,up);

  // open top claw
  Serial.print("\n 9 Open top Claw");
  servo_top.write(topen);
  delay(1000);
  servo_top.write(90);
  
// Reposition top claw to above cg of stack
Serial.print("\n 10 Moving Down top claw");
   //myStepper.step(d2s(pos-(stack/2),down));
   //stepper.setSpeed(d2s(pos-(stack/2),down));
   //stepper.runSpeed();
   t = d2s(stack,down);
   total_s(t,down);
   moveStepper(total_steps,speed_stepper,down);
  delay(1500);

// Close top claw
Serial.print("\n 11 Closing top Claw");
  servo_top.write(tclose);
  delay(500);
  servo_top.write(90);
  */
  
// open bottom claw
Serial.print("\n 12 Closing bottom Claw");
  servo_bot.write(bopen);

// Raise stack above bottom claw
Serial.print("\n 13 Moving top claw Up");
  //myStepper.step(d2s(pos+1,up));
   //stepper.setSpeed(d2s(pos+1,up));
   //stepper.runSpeed();
   t = d2s(pos+1,up);
   total_s(t,up);
   moveStepper(total_steps,speed_stepper,up);
  delay(1500);


}



void loop() 
{

  int i = 0;
 Serial.print("Current position ");
 Serial.println(stepper.currentPosition());
 Serial.print(total_steps);
 delay(2000);
 //firstStack();
 if(stack == 0)
 {
  //int t = d2s(3,up);
  // total_s(t,up);
// moveStepper(total_steps,800,up);
  firstStack();
   moveStepper(total_steps,800,down);
  //stack++;
 }

 
 else if(stack<5)
 {
  bstack();
 }
 /*else
 {
  lastStack();
 }
 actualStack = actualStack + stack;
 Serial.print("Actual stack: ");
 Serial.println(actualStack);
*/
}
