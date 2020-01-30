/*
  FILE :  line_following_ds_v1_0112502
  DESCRIPTION: SIMPLE LINE FOLLOWING ROBOT
  AUTHOR : ISABEL BARNOLA
  DATE: 
  REFERENCES:
    https://create.arduino.cc/projecthub/robocircuits/line-follower-robot-arduino-299bae

    USING SENSOR ANALOG READ 
    EXAMPLE USES WHITE SURFACE AND BLACK LINES
    TEST CURRENT SET UP AND THEN CHANGE VALUES
*/

// ================================================================
// H BRIDGE 
// ================================================================

#define motor_speed 200 // MAX = 255
#define BLACK 150
int turn_speed = 230;    // MAX 255 
int turn_delay = 10;

//-----------------------
// Motor A: Right
//-----------------------

int motor_A_en  = 46;   //Enable Pin of the Right Motor (must be PWM)
int motor_A_in1 = 53;   //Control Pin
int motor_A_in2 = 52;

//-----------------------
// Motor B: Left
//-----------------------

int motor_B_en  = 45;   //Enable Pin of the Left Motor (must be PWM)
int motor_B_in1 = 51;
int motor_B_in2 = 50;

// ================================================================
// Setup
// ================================================================

void setup() 
{
  // put your setup code here, to run once:
  
  //-----------------------
  // Motors
  //-----------------------

  // A: Right
  pinMode(motor_A_en,  OUTPUT);  // Speed
  pinMode(motor_A_in1, OUTPUT);
  pinMode(motor_A_in2, OUTPUT);
  // B: Left
  pinMode(motor_B_en,  OUTPUT);  // Speed
  pinMode(motor_B_in1, OUTPUT);
  pinMode(motor_B_in2, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite (motor_A_in1,LOW);
    digitalWrite (motor_A_in2,HIGH);                       
    digitalWrite (motor_B_in1,LOW);
    digitalWrite (motor_B_in2,HIGH);
  
    digitalWrite  (motor_A_en, motor_speed);
    digitalWrite  (motor_B_en, motor_speed);
}
