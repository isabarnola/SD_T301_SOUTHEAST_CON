/*
  FILE :  line_following_v1_0112454
  DESCRIPTION: SIMPLE LINE FOLLOWING ROBOT
  AUTHOR : ISABEL BARNOLA
  REFERENCES:
    https://create.arduino.cc/projecthub/robocircuits/line-follower-robot-arduino-299bae

    USING SENSOR ANALOG READ 
    EXAMPLE USES WHITE SURFACE AND BLACK LINES
    TEST CURRENT SET UP AND THEN CHANGE VALUES
*/


int vSpeed = 110;        // MAX 255
int turn_speed = 230;    // MAX 255 
int turn_delay = 10;
  
// H BRIDGE  
const int motorA1      = 5;  
const int motorA2      = 3; 
const int motorAspeed  = 6;
const int motorB1      = 4; 
const int motorB2      = 2; 
const int motorBspeed  = 1;

//Sensor Connection
const int left_sensor_pin  = A0;
const int right_sensor_pin = A1;
  
int left_sensor_state;
int right_sensor_state;

void setup() 
{
  // motor A pin setup
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  // motor B pin setup
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  Serial.begin(9600);

  delay(3000); 
}

void loop() 
{
  
  left_sensor_state =  analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);

  // RIGHT
  //(right_sensor_state==HIGH && left_sensor_state==LOW)
  if(right_sensor_state > 500 && left_sensor_state < 500)
  {
    Serial.println("turning right");
  
    digitalWrite (motorA1,LOW);
    digitalWrite (motorA2,HIGH);                       
    digitalWrite (motorB1,LOW);
    digitalWrite (motorB2,HIGH);
  
    analogWrite  (motorAspeed, vSpeed);
    analogWrite  (motorBspeed, turn_speed);
    
   }

  // LEFT
  //(right_sensor_state==LOW  && left_sensor_state==HIGH)
  if(right_sensor_state < 500 && left_sensor_state > 500)
  {
    Serial.println("turning left");
    
    digitalWrite (motorA1,HIGH);
    digitalWrite (motorA2,LOW);                       
    digitalWrite (motorB1,HIGH);
    digitalWrite (motorB2,LOW);
  
    analogWrite (motorAspeed, turn_speed);
    analogWrite (motorBspeed, vSpeed);
  
    delay(turn_delay);
   }

  // FORWARD
  if(right_sensor_state > 500 && left_sensor_state > 500)
  {
    Serial.println("going forward");
  
    digitalWrite (motorA2,LOW);
    digitalWrite (motorA1,HIGH);                       
    digitalWrite (motorB2,HIGH);
    digitalWrite (motorB1,LOW);
  
    analogWrite (motorAspeed, vSpeed);
    analogWrite (motorBspeed, vSpeed);
  
    delay(turn_delay);
    
   }
   
  // STOP
  if(right_sensor_state < 500 && left_sensor_state < 500)
  { 
    Serial.println("stop");
    
    analogWrite (motorAspeed, 0);
    analogWrite (motorBspeed, 0);
    
    }
  
   
}
