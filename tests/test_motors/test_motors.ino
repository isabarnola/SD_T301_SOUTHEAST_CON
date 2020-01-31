
#define motor_speed 200 // MAX = 255
#define BLACK 150
int turn_speed = 150;    // MAX 255 
int turn_delay = 10;

//-----------------------
// Motor A: Right
//-----------------------

int motor_A_en  = 7;   //Enable Pin of the Right Motor (must be PWM)
int motor_A_in1 = 52;   //Control Pin
int motor_A_in2 = 53;

//-----------------------
// Motor B: Left
//-----------------------

  int motor_B_en  = 4;   //Enable Pin of the Left Motor (must be PWM)
  int motor_B_in1 = 50;
  int motor_B_in2 = 51;

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
  //pinMode(motor_A_en,  OUTPUT);  // Speed
  pinMode(motor_A_in1, OUTPUT);
  pinMode(motor_A_in2, OUTPUT);
  // B: Left
  //pinMode(motor_B_en,  OUTPUT);  // Speed
  pinMode(motor_B_in1, OUTPUT);
  pinMode(motor_B_in2, OUTPUT);


}

void loop() 
{
  
    digitalWrite (motor_A_in1,HIGH);
    digitalWrite (motor_A_in2,LOW);                       
    digitalWrite (motor_B_in1,LOW);
    digitalWrite (motor_B_in2,HIGH);
  
    analogWrite  (motor_A_en, motor_speed);
    //analogWrite  (motor_B_en, motor_speed);
}
