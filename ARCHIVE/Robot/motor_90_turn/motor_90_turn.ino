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

// ================================================================
// H BRIDGE 
// ================================================================
#define ENCODEROUTPUT 4741.44
#define MIN 80
#define motor_speed 100 // MAX = 255
int motor_speed_R  = 85;// MAX = 255
int motor_speed_L =95; // MAX = 255
#define turn_speed_r  90//95
#define turn_speed_l  85//100
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
int ENC_RA = 2;
int ENC_RB = 35;
int ENC_LA = 3;
int ENC_LB = 31;
//-----------------------
// Motor : Right
//-----------------------
int R_ME  = 4;   //Enable Pin of the Right Motor (must be PWM) orange wire
int R_M1 = 50;    //Control Pin yellow wire

//-----------------------
// Motor : Left
//-----------------------
int L_ME  = 6;   //Enable Pin of the Left Motor (must be PWM) green wire
int L_M1 = 51;   // Control Pin blue wire 

int dir = 0;
int cnt = 0;

long previousMillis = 0;
long currentMillis = 0;
int interval = 1000;
long turn_t_r = 0;
long turn_t_l = 0;
long TIME_TR = 1000;
long TIME_TL = 420;


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
  
  digitalWrite(L_M1,LOW);
  digitalWrite(R_M1,LOW);
  analogWrite (L_ME,a);  //PWM Speed Control
  analogWrite (R_ME,b);
  
}
void back_off (char a,char b)          //Move backward
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,HIGH);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,HIGH);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (R_ME,a);
  analogWrite (L_ME,b);
  delay(2);
  digitalWrite(R_M1,HIGH);
  digitalWrite(L_M1,LOW);
  delay(2);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,LOW);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,HIGH);
}

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
  //attachInterrupt(1, ISR_countL, CHANGE); 

  //attachInterrupt(digitalPinToInterrupt (ENC_RA), ISR_countR, RISING);  
  // Increase counter A when speed sensor pin goes High
  //attachInterrupt(digitalPinToInterrupt (ENC_LA), ISR_countL, RISING);  
  // Increase count

  //previousMillis = millis();

  // Sets the data rate in bits 
  // per second (baud) for serial data transmission.
  Serial.begin(9600); 

}

void loop() 
{
  
  

  while (turn_t_l < TIME_TL)
  {
    Serial.print("previosmillis ");
    Serial.println(previousMillis);
    Serial.print("currentMillis ");
    currentMillis= millis();
    Serial.println(currentMillis);
    Serial.println("rotating 90 degrees left");
    turn_L(turn_speed_r,turn_speed_l);
    turn_t_l = turn_t_l + currentMillis;
    Serial.print(" time passed: ");
    Serial.print(turn_t_l);
    Serial.print("of ");
    Serial.println(TIME_TL);
    
    previousMillis = currentMillis;
  }
  Serial.println("done");
  stopp();
  //delay(5);
}
