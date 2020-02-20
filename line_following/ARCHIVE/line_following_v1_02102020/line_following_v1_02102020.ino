/*
  FILE :  line_following_ds_v1_0112502
  DESCRIPTION: SIMPLE LINE FOLLOWING ROBOT
  AUTHOR : ISABEL BARNOLA
  DATE: 02/05
  REFERENCES:
    https://create.arduino.cc/projecthub/robocircuits/line-follower-robot-arduino-299bae

    USING SENSOR ANALOG READ 
    EXAMPLE USES WHITE SURFACE AND BLACK LINES
    TEST CURRENT SET UP AND THEN CHANGE VALUES
*/
#include "encoder.h"
// ================================================================
// H BRIDGE 
// ================================================================
#define ENCODEROUTPUT 4741.44
#define MIN 80
#define motor_speed 100 // MAX = 255
int motor_speed_R  = 105;//95;//85;// MAX = 255
int motor_speed_L =115;//105;//95; // MAX = 255
#define turn_speed  85
#define BLACK 150
volatile float counter_R = 0;
volatile float counter_L = 0;
unsigned int tempR;
unsigned int tempL;
float newtime;
float oldtime = 0;
unsigned int old_counter_R = 0;
unsigned int old_counter_L = 0;
float velR;
float velL;
int turn_delay = 10;
/*
int ENC_RA = 2;
int ENC_RB = 35;
int ENC_LA = 3;
int ENC_LB = 31;
*/
//-----------------------
// Motor : Right
//-----------------------
int R_M_PWM  = 4;   //Enable Pin of the Right Motor (must be PWM)
int R_M_E = 50;    //Control Pin

//-----------------------
// Motor : Left
//-----------------------
int L_M_PWM  = 6;   //Enable Pin of the Left Motor (must be PWM)
int L_M_E = 51;

int dir = 0;
int cnt = 0;

long previousMillis = 0;
long currentMillis = 0;
int interval = 1000;

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
  analogWrite(R_M_PWM,0);
  digitalWrite(R_M_E,LOW);
  analogWrite(L_M_PWM,0);
  digitalWrite(L_M_E,LOW);
}
void advance(char a,char b)          //Move forward
{
  
  digitalWrite(L_M_E,LOW);
   //delay(1);
  digitalWrite(R_M_E,LOW);
  // delay(1);
  analogWrite (L_M_PWM,a);  //PWM Speed Control
    //delay(1);
  analogWrite (R_M_PWM,b);
  delay(1);
  
}
void back_off (char a,char b)          //Move backward
{
  analogWrite (R_M_PWM,a);
  digitalWrite(R_M_E,HIGH);
  analogWrite (L_M_PWM,b);
  digitalWrite(L_M_E,HIGH);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (R_M_PWM,a);
  digitalWrite(R_M_E,HIGH);
  analogWrite (L_M_PWM,b);
  digitalWrite(L_M_E,LOW);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (R_M_PWM,a);
  digitalWrite(R_M_E,LOW);
  analogWrite (L_M_PWM,b);
  digitalWrite(L_M_E,HIGH);
}
/*
// Motor A pulse count ISR
void ISR_countR()  
{
  //Serial.print(counter_R);
  //counter_R++;  // increment Motor A counter value
  if (digitalRead(ENC_RA) == HIGH) 
  {
    
      counter_R++;
    
  }
  if (digitalRead(ENC_LA) == HIGH) 
  {
    
      counter_L++;
  }
   
 
}
// Motor B pulse count ISR
void ISR_countL()  
{
   //Serial.print(counter_L);
  //counter_L++;  // increment Motor B counter value
  if (digitalRead(ENC_LA) == HIGH) 
  {
   
      counter_L++;
   
  }
}
*/
// ================================================================
// Setup
// ================================================================

void setup() 
{
  // put your setup code here, to run once:
/*
pinMode(ENC_RA,INPUT);
pinMode(ENC_RB,INPUT);
pinMode(ENC_LA,INPUT);
pinMode(ENC_LB,INPUT);
 */
  //-----------------------
  // Motors
  //-----------------------

  // Right
  pinMode(R_M_E, OUTPUT);

  // Left
  pinMode(L_M_E, OUTPUT);


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
	//attachInterrupt(0, ISR_countR, CHANGE);  
  	 // Increase counter B when speed sensor pin goes High
	//attachInterrupt(1, ISR_countL, CHANGE); 

  //attachInterrupt(digitalPinToInterrupt (ENC_RA), ISR_countR, RISING);  
  // Increase counter A when speed sensor pin goes High
  //attachInterrupt(digitalPinToInterrupt (ENC_LA), ISR_countL, RISING);  
  // Increase count

  //previousMillis = millis();

  // Sets the data rate in bits 
  // per second (baud) for serial data transmission.
  encoder_init();
  Serial.begin(9600); 

}

void loop() 
{
  
  
  /*
  Serial.println("================ NEW CODE 628 ================");
  Serial.print("previosmillis ");
  Serial.println(previousMillis);
  Serial.print("currentMillis ");
  currentMillis= millis();
  Serial.println(currentMillis);
  */
  //-----------------------
  // IR Sensors: READ
  //-----------------------
  left_sensor_state =  analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);

   Serial.print("===================counL: ");
    Serial.println(encoder1_val);
    Serial.print("=====================counR: ");
    Serial.println(encoder0_val);
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
    if (dir != 1)
    {
       dir = 1;
       cnt = 0;
    }

    Serial.println("going forward");
    advance(motor_speed_R,motor_speed_L);

   
  /*
    velR = counter_R /(currentMillis - previousMillis);
    velL = counter_L /(currentMillis - previousMillis);
    
    Serial.print("===============================velR: ");
    Serial.println(velR);
    Serial.print("===============================velL: ");
    Serial.println(velL);

      if (velR > velL && motor_speed_R > MIN && motor_speed_L> MIN)
      {
         motor_speed_L--;
        Serial.println("--------------------changing left");
        
       
      }
      else if (velL >  velR && motor_speed_R > MIN && motor_speed_L> MIN)
      {
       motor_speed_R--;
        Serial.println("--------------------changing right");
        
      }
      
       
        Serial.print("Right speed:");
        Serial.println( motor_speed_R);
         Serial.print("left speed:");
        Serial.println(motor_speed_L);
  
        cnt++;
       
        counter_R = 0;
        counter_L = 0;
        
    
     
  */
   
  }
  // STOP
  if(right_sensor_state < BLACK && left_sensor_state < BLACK)
  { 
     Serial.println("stop");
     stopp();
    
  }
   previousMillis = currentMillis;
  //delay(500);
}
