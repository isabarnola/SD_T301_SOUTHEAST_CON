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

int motor_A_en  = 3;   //Enable Pin of the Right Motor (must be PWM)
int motor_A_in1 = 1;   //Control Pin
int motor_A_in2 = 2;

//-----------------------
// Motor B: Left
//-----------------------

int motor_B_en  = 6;   //Enable Pin of the Left Motor (must be PWM)
int motor_B_in1 = 4;
int motor_B_in2 = 5;

// ================================================================
// IR Sensor
// ================================================================

//-----------------------
// Sensor: Left
//-----------------------

const int left_sensor_pin  = A0;
int left_sensor_state;

//-----------------------
// Sensor: Right
//-----------------------

const int right_sensor_pin = A1;
int right_sensor_state;

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

  //-----------------------
  // IR Sensors
  //-----------------------
  
  // Right
  pinMode(right_sensor_pin,   INPUT);
  //pinMode(right_sensor_state, INPUT);

  // Left
  pinMode(left_sensor_pin,    INPUT);
  //pinMode(right_sensor_state, OUTPUT);

  // Sets the data rate in bits 
  // per second (baud) for serial data transmission.
  Serial.begin(9600); 

}

void loop() 
{

  //-----------------------
  // IR Sensors: READ
  //-----------------------
  left_sensor_state =  analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);


   //-----------------------
  // Navigation
  //-----------------------
  
  // RIGHT
  //(right_sensor_state==HIGH && left_sensor_state==LOW)
  if(right_sensor_state < BLACK && left_sensor_state > BLACK)
  {
    Serial.println("turning right");
  
    digitalWrite (motor_A_in1,LOW);
    digitalWrite (motor_A_in2,HIGH);                       
    digitalWrite (motor_B_in1,LOW);
    digitalWrite (motor_B_in2,HIGH);
  
    analogWrite  (motor_A_en, motor_speed);
    analogWrite  (motor_B_en, turn_speed);
    
   }

  // LEFT
  //(right_sensor_state==LOW  && left_sensor_state==HIGH)
  if(right_sensor_state > BLACK && left_sensor_state < BLACK)
  {
    Serial.println("turning left");
    
    digitalWrite (motor_A_in1,HIGH);
    digitalWrite (motor_A_in2,LOW);                       
    digitalWrite (motor_B_in1,HIGH);
    digitalWrite (motor_B_in2,LOW);
  
    analogWrite (motor_A_en, turn_speed);
    analogWrite (motor_B_en, motor_speed);
  
    delay(turn_delay);
   }

  // FORWARD
  if(right_sensor_state > BLACK && left_sensor_state > BLACK)
  {
    Serial.println("going forward");
  
    digitalWrite (motor_A_in2,LOW);
    digitalWrite (motor_A_in1,HIGH);                       
    digitalWrite (motor_B_in2,HIGH);
    digitalWrite (motor_B_in1,LOW);
  
    analogWrite (motor_A_en, motor_speed);
    analogWrite (motor_B_en, motor_speed);
  
    delay(turn_delay);
    
   }
   
  // STOP
  if(right_sensor_state < BLACK && left_sensor_state < BLACK)
  { 
    Serial.println("stop");
    
    analogWrite (motor_A_en, 0);
    analogWrite (motor_B_en, 0);
    
    }
  

}
