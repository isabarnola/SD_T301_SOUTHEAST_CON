/*
  FILE :  line_following_ds_v1_0112502
  DESCRIPTION: SIMPLE LINE FOLLOWING ROBOT
  AUTHOR : ISABEL BARNOLA
  DATE: 02/05
  REFERENCES:
    https://create.arduino.cc/projecthub/robocircuits/line-follower-robot-arduino-299bae
    USING SENSOR ANALOG READ 
    EXAMPLE USES WHITE SURFACE AND THRESHOLD LINES
    TEST CURRENT SET UP AND THEN CHANGE VALUES
*/


// ================================================================
// H BRIDGE 
// ================================================================
#define ENCODEROUTPUT 4741.44
#define MIN 80
#define motor_speed 100 // MAX = 255
int motor_speed_R  = 80;//95;//85;// MAX = 255
int motor_speed_L =59;//105;//95; // MAX = 255
#define turn_speed  45
#define turn_speed1 55
#define THRESHOLD 330
#define L_Forward LOW
#define R_Forward HIGH
#define L_Backward HIGH
#define R_Backward LOW
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
int R_ME  = 6;   //Enable Pin of the Right Motor (must be PWM)
int R_M1 = 51;    //Control Pin

//-----------------------
// Motor : Left
//-----------------------
int L_ME  = 4;   //Enable Pin of the Left Motor (must be PWM)
int L_M1 = 50;

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

const int left_sensor_pin  = A1;
int left_sensor_state;

//-----------------------
// Sensor: Right
//-----------------------

const int right_sensor_pin = A0;
int right_sensor_state;

//-----------------------
// Sensor: Middle
//-----------------------
const int middle_sensor_pin = A2;
int middle_sensor_state;

//-----------------------
// Sensor: Left
//-----------------------

//const int left2_sensor_pin  = A3;
//int left2_sensor_state;

// ================================================================
// Function
// ================================================================

void stopp(void)                    //Stop
{
  analogWrite(R_ME,0);
  digitalWrite(R_M1,LOW);
  analogWrite(L_ME,0);
  digitalWrite(L_M1,LOW);
}
void advance(char a,char b)          //Move forward
{
  
  digitalWrite(L_M1,L_Forward);
   //delay(1);
  digitalWrite(R_M1,R_Forward);
  // delay(1);
  analogWrite (L_ME,a);  //PWM Speed Control
    //delay(1);
  analogWrite (R_ME,b);
  delay(1);
  
}
void read_ir()
{
  left_sensor_state   =  analogRead(left_sensor_pin);
  right_sensor_state  = analogRead(right_sensor_pin);
  middle_sensor_state  =  analogRead(middle_sensor_pin);
  //right2_sensor_state = analogRead(right2_sensor_pin);
   
}
void back_off (char a,char b)          //Move backward
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Backward);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Backward);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Forward);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Backward);
}
void turn_L_line (char a)             //Turn Left
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Forward);
  analogWrite (L_ME,0);
  digitalWrite(L_M1,LOW);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Backward);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Forward);
}
void turn_R_line (char b)             //Turn Right
{
  analogWrite (R_ME,0);
  digitalWrite(R_M1,LOW);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Forward);
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
void ISR_IR()
{
  left_sensor_state =  analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);
  middle_sensor_state = analogRead(middle_sensor_pin);
  
  }
int count = 0;

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
  attachInterrupt(20, ISR_countR, CHANGE);  
  //attachInterrupt(21,ISR_IR, CHANGE);
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
int passed = 2;
int n = 2;

  int white_lines = 0;
void loop() 
{
  
  
  /*
  Serial.println("================ NEW CODE 628 ================");
  Serial.print("previosmillis ");
  Serial.println(previousMillis);
  Serial.print("currentMillis ");
  */
  currentMillis= millis();
 // Serial.println(currentMillis);
  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();
   
   Serial.print(left_sensor_state);
   Serial.print("\t");
  
   Serial.print("\t");
   //Serial.print(right2_sensor_state);
   Serial.print("\t");
   Serial.println(right_sensor_state);
  
  
  
Serial.print("-------------------------------------------------------------------White lines passed ");
Serial.println(count);
   //-----------------------
  // Navigation
  //-----------------------

  
  // RIGHT
  //      | | |  
  //       L M R
  //      | | |

  // ->
  // L(1) M(1) R(0)
  if(right_sensor_state < THRESHOLD   && middle_sensor_state < THRESHOLD && left_sensor_state > THRESHOLD) 
   //remember to check
  {
    Serial.println("turning right L(1) M(1) R(0) ");
    turn_L_line(turn_speed);
   }
    //-----------------------
    // IR Sensors: READ
    //-----------------------
     read_ir();
     
    // L(0) M(0) R(1)
   
    //      | | |  
    //       R
    //     M  | |
    //   L  | | |  

    //->
   if (right_sensor_state < THRESHOLD && middle_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD)
   {
    
    Serial.println("going left L(0) M(0) R(1)");
    turn_L_line(turn_speed1);
   }

  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();

  // LEFT
  //     ||||   
  //      L2||2R
  //      ||||
  // L(1) M(1) R(0)
  // <-
  if(right_sensor_state > THRESHOLD && middle_sensor_state < THRESHOLD && left_sensor_state < THRESHOLD)
  {
    Serial.println("going left");
    turn_R_line(turn_speed);
   }

  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();
   
   // L(1) M(0) R(0)
   //      | | |  
   //        L  
   //      | |  M 
   //      | |    R 
   // ->
   if (right_sensor_state > THRESHOLD   && middle_sensor_state > THRESHOLD && left_sensor_state < THRESHOLD)
   {
    Serial.println("turning right L(1) M(0) R(0) ");
    turn_R_line(turn_speed1);
    }
    
  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();
  
  // STOP
   if(right_sensor_state < THRESHOLD && middle_sensor_state < THRESHOLD && left_sensor_state < THRESHOLD){
     if (white_lines == n)
     {
        Serial.println("stop");
        stopp();
     }
     else
     {
      while(right_sensor_state < THRESHOLD && left_sensor_state < THRESHOLD && middle_sensor_state < THRESHOLD)
      {
          read_ir();
         Serial.print("|||||||||||||||| PASSING WHITE||||||||||||||||||  ");
         Serial.println(white_lines);
        advance(motor_speed_R,motor_speed_L);
        Serial.print("=====================================================right_sensor_state ");
        Serial.println(right_sensor_state);
        Serial.print("=====================================================left_sensor_state");
        Serial.println(left_sensor_state);

     /* Serial.print("===================counL: ");
    Serial.println(counter_L);
    Serial.print("=====================counR: ");
    Serial.println(counter_R);
    */
  
    velR = counter_R /(currentMillis - previousMillis);
    velL = counter_L /(currentMillis - previousMillis);
    /*
    Serial.print("===============================velR: ");
    Serial.println(velR);
    Serial.print("===============================velL: ");
    Serial.println(velL);
    */

      if (velR > velL && motor_speed_R > MIN && motor_speed_L> MIN)
      {
         motor_speed_L--;
        //Serial.println("--------------------changing left");
        
       
      }
      else if (velL >  velR && motor_speed_R > MIN && motor_speed_L> MIN)
      {
       motor_speed_R--;
        //Serial.println("--------------------changing right");
        
      }
      
       /*
        Serial.print("Right speed:");
        Serial.println( motor_speed_R);
         Serial.print("left speed:");
        Serial.println(motor_speed_L);
  */
        cnt++;
       
        counter_R = 0;
        counter_L = 0;
        
    
     
      }
      white_lines++;
      count++;
      Serial.print("-------------------------------------lines passed: ");
      Serial.println(white_lines);
     }
     
    
  }

  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();

  // FORWARD
  if(right_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD && middle_sensor_state)
  
  {
    if (dir != 1)
    {
       dir = 1;
       cnt = 0;
    }

    Serial.println("going forward");
    advance(motor_speed_R,motor_speed_L);

    /*  Serial.print("===================counL: ");
    Serial.println(counter_L);
    Serial.print("=====================counR: ");
    Serial.println(counter_R);
  */
    velR = counter_R /(currentMillis - previousMillis);
    velL = counter_L /(currentMillis - previousMillis);
    /*
    Serial.print("===============================velR: ");
    Serial.println(velR);
    Serial.print("===============================velL: ");
    Serial.println(velL);
*/
      if (velR > velL && motor_speed_R > MIN && motor_speed_L> MIN)
      {
         motor_speed_L--;
        //Serial.println("--------------------changing left");
        
       
      }
      else if (velL >  velR && motor_speed_R > MIN && motor_speed_L> MIN)
      {
       motor_speed_R--;
       // Serial.println("--------------------changing right");
        
      }
      
       /*
        Serial.print("Right speed:");
        Serial.println( motor_speed_R);
         Serial.print("left speed:");
        Serial.println(motor_speed_L);
  */
        cnt++;
       
        counter_R = 0;
        counter_L = 0;
        
    
     
  
   
  }
   previousMillis = currentMillis;
  //delay(500);
}
