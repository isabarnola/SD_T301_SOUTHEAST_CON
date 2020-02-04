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

#define motor_speed 100 // MAX = 255
int motor_speed_R  = 100;// MAX = 255
int motor_speed_L =100; // MAX = 255
#define turn_speed  85
#define BLACK 150
volatile int counter_R = 0;
volatile int counter_L = 0;
int turn_delay = 10;

int ENC_RA = 30;
int ENC_RB = 31;
int ENC_LA = 29;
int ENC_LB = 28;
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
  analogWrite (R_ME,a);             //PWM Speed Control
  analogWrite (L_ME,b);
  digitalWrite(R_M1,HIGH);
  digitalWrite(L_M1,HIGH);
  
  //delay(2);                 //10KHz Frequency
  digitalWrite(R_M1,LOW);
  digitalWrite(L_M1,LOW);
  //delay(5);                //10KHz Frequency
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

// Motor A pulse count ISR
void ISR_countR()  
{
  //Serial.print(counter_R);
  //counter_R++;  // increment Motor A counter value
  if (digitalRead(ENC_RA) == HIGH) 
  {
    if (digitalRead(ENC_RB) == LOW) 
    {
      counter_R++;
    } 
    else 
    {
      counter_R--;
    }
  } 
  else
  {
    if (digitalRead(ENC_RB) == LOW) 
    {
      counter_R--;
    } 
    else 
    {
      counter_R++;
    }
} 
}
// Motor B pulse count ISR
void ISR_countL()  
{
   //Serial.print(counter_L);
  //counter_L++;  // increment Motor B counter value
  if (digitalRead(ENC_LA) == HIGH) 
  {
    if (digitalRead(ENC_LB) == LOW) 
    {
      counter_L++;
    } 
    else 
    {
      counter_L--;
    }
  } 
  else
  {
    if (digitalRead(ENC_LB) == LOW) 
    {
      counter_L--;
    } 
    else 
    {
      counter_L++;
    }
}
}

// ================================================================
// Setup
// ================================================================

void setup() 
{
  // put your setup code here, to run once:

pinMode(ENC_RA,INPUT);
pinMode(ENC_RB,INPUT);
pinMode(ENC_LA,INPUT);
pinMode(ENC_LB,INPUT);
  
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

  //----------------------
  // Encoders
  //----------------------

  // Increase counter A when speed sensor pin goes High
	attachInterrupt(0, ISR_countR, CHANGE);  
  	 // Increase counter B when speed sensor pin goes High
	attachInterrupt(1, ISR_countL, CHANGE); 



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
    turn_R(motor_speed_R,motor_speed_L);
    
   }

  // LEFT
  //(right_sensor_state==LOW  && left_sensor_state==HIGH)
  if(right_sensor_state > BLACK && left_sensor_state < BLACK)
  {
    Serial.println("going left");
    turn_L(motor_speed_R,motor_speed_L);
   
   }

  // FORWARD
  if(right_sensor_state > BLACK && left_sensor_state > BLACK)
  {
    Serial.println("going forward");
    advance(motor_speed_R,motor_speed_L);
    Serial.print("Right encoder: ");
    Serial.println(counter_R);
    Serial.print("left encoder: ");
    Serial.println(counter_L);
    if (counter_L > counter_R && motor_speed_R > 90 && motor_speed_L> 80)
    {
      motor_speed_L--;
      }
     
      Serial.print("Right speed:");
      Serial.println( motor_speed_R);
       Serial.print("left speed:");
      Serial.println(motor_speed_L);
      
      
   }
   
  // STOP
  if(right_sensor_state < BLACK && left_sensor_state < BLACK)
  { 
     Serial.println("stop");
     stopp();
    
  }
  delay(500);

}
