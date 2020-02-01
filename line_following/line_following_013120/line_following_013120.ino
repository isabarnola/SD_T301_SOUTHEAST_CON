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

#define motor_speed 80 // MAX = 255
#define turn_speed  80
#define BLACK 150
 
int turn_delay = 10;

//-----------------------
// Motor : Right
//-----------------------
int R_ME  = 4;   //Enable Pin of the Right Motor (must be PWM)
int R_M1 = 50;    //Control Pin

//-----------------------
// Motor : Left
//-----------------------
int L_ME  = 6;   //Enable Pin of the Left Motor (must be PWM)
int L_M1 = 51;


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
// Function
// ================================================================

void stopp(void)                    //Stop
{
  digitalWrite(R_ME,0);
  digitalWrite(R_M1,LOW);
  digitalWrite(L_ME,0);
  digitalWrite(L_M1,LOW);
}
void advance(char a,char b)          //Move forward
{
  analogWrite (R_ME,a);      //PWM Speed Control
  digitalWrite(R_M1,HIGH);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,HIGH);
}
void back_off (char a,char b)          //Move backward
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,LOW);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,LOW);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,LOW);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,HIGH);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,HIGH);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,LOW);
}


// ================================================================
// Setup
// ================================================================

void setup() 
{
  // put your setup code here, to run once:
  
  //-----------------------
  // Motors
  //-----------------------

  // Right
  pinMode(R_M1, OUTPUT);

  // Left
  pinMode(L_M1, OUTPUT);


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
    turn_R(motor_speed,motor_speed);
    
   }

  // LEFT
  //(right_sensor_state==LOW  && left_sensor_state==HIGH)
  if(right_sensor_state > BLACK && left_sensor_state < BLACK)
  {
    Serial.println("going left");
    turn_L(motor_speed,motor_speed);
   
   }

  // FORWARD
  if(right_sensor_state > BLACK && left_sensor_state > BLACK)
  {
    Serial.println("going forward");
    advance(motor_speed,motor_speed);
      
   }
   
  // STOP
  if(right_sensor_state < BLACK && left_sensor_state < BLACK)
  { 
     Serial.println("going stop");
     //stopp();
    advance(motor_speed,motor_speed);
  }
  

}
