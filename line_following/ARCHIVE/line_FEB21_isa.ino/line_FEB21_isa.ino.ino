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

#include "mrobot.h"

// ================================================================
// H BRIDGE 
// ================================================================
#define ENCODEROUTPUT 4741.44
#define MIN 80
#define motor_speed 100 // MAX = 255
int speed_R  = 78;//95;//85;// MAX = 255
int speed_L =59;//105;//95; // MAX = 255
#define turn_speed  45
#define turn_speed1 55
#define THRESHOLD 330
#define BLACK HIGH
#define WHITE LOW
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
/*
int ENC_RA = 2;
int ENC_RB = 20;//35;
int ENC_LA = 3;
int ENC_LB = 21; // 31;
*/
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

void go_straight()
{
  int temp1 = abs(cur_wvel[1] -  cur_wvel[0]);
  
  if (temp1 >= 0.05 && temp1 <= 0.5 )
  {
    speed_L =speed_L;
    }
   else if (cur_wvel[1] > cur_wvel[0] )
      speed_L--;
    else if (cur_wvel[1] < cur_wvel[0])
      speed_L++;
}

void turn_90_r(int right, int left, int time)
{
  analogWrite (R_ME,right);
  analogWrite (L_ME,left);
  int TimeMillis = 0;
 
  while(TimeMillis - time < 1000)
   {                  //Time to turn towards bin
    Serial.println("---------------------Time to turn:  ");
    Serial.println(TimeMillis - time);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
}

void turn_r_line()
{
  read_ir();
  while(left_sensor_state != BLACK && middle_sensor_state != BLACK && left_sensor_state != BLACK)
  {
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    read_ir();
  }

  //while()

}

int count = 0;

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
  pinMode(R_M1, OUTPUT);  // Right
  pinMode(L_M1, OUTPUT); // Left

  //-----------------------
  // IR Sensors
  //-----------------------
  pinMode(right_sensor_pin,   INPUT);  // Right
  pinMode(left_sensor_pin,    INPUT);   // Left
  
  //----------------------
  // Encoders
  //----------------------
  encoder_init();

  Serial.begin(9600); 

}

int passed = 2;
int n = 2;
int white_lines = 0;

void loop() 
{
  
  currentMillis= millis();

  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();
    
  Serial.print("---------------------------------------------------------------White lines passed ");
  Serial.println(count);
  
 
   
  if (count >= 0)
  {
    //------------------------------------------
    // Navigation
    //------------------------------------------

    // RIGHT
    //      | | |  
    //       L M R
    //      | | |
    // ->
    // L(W) M(W) R(B)
    if(right_sensor_state == WHITE      && middle_sensor_state == WHITE    && left_sensor_state == BLACK)
    {
      Serial.println("turning right L(W) M(W) R(B) ");
      turn_L_line(turn_speed);
      
    }
    else
    {  // opening else - right eft - left - stop - forward 
        read_ir();
        //      | | |  
        //       R
        //     M  | |
        //   L  | | |  
        //->
        // L(B) M(B) R(W)
      if (left_sensor_state == BLACK       && middle_sensor_state == BLACK    &&  right_sensor_state == WHITE)
      {
        Serial.println("going left L(B) M(B) R(W)");
        turn_L_line(turn_speed1);
      }
      else
      {// opening else - left - left - stop - forward
        read_ir();
        // LEFT
        //     ||||   
        //      L2||2R
        //      ||||
        // L(W) M(W) R(B)
        // <-
          if(left_sensor_state == WHITE     && middle_sensor_state == WHITE    && right_sensor_state == BLACK )
        {// opening if - go left
          Serial.println("going left");
          turn_R_line(turn_speed);
        }// close if - go left
        else
        { // opening else - left - stop - forward
          read_ir();
          // L(W) M(B) R(B)
          //      | | |  
          //        L  
          //      | |  M 
          //      | |    R 
          // <-
          if (left_sensor_state == WHITE && middle_sensor_state == BLACK && right_sensor_state == BLACK)
          { // opening if - going left
            Serial.println("turning right L(1) M(0) R(0) ");
            turn_R_line(turn_speed1);
          } // closing if - going left
          else
          { // opening else - stop & forward
            read_ir();
            // STOP
            if(right_sensor_state < THRESHOLD && middle_sensor_state < THRESHOLD && left_sensor_state < THRESHOLD)
            { // opening if - stop
              if (white_lines == n)
              { // opening if - stop
                  Serial.println("stop          =       =       =     =         =       =       =       =       =     =");
                  stopp();
                  
                  count = -1;
              } // closing if - stop
              else
              { // opening else - pass white line
                while(right_sensor_state < THRESHOLD && left_sensor_state < THRESHOLD && middle_sensor_state < THRESHOLD)
                { // opening while loop - pass white line
                      read_ir();
                    Serial.print("|||||||||||||||| PASSING WHITE||||||||||||||||||  ");
                    Serial.println(white_lines);
                    get_current_status();     // get velocity
                 
                    advance(speed_R,speed_L); // move forward
                    go_straight();            // correct velocity
                } // closing while loop - pass white line
                white_lines++;
               
                
                count++;
                Serial.print("-------------------------------------lines passed: ");
                Serial.println(white_lines);
              } // close else - pass white line
            } // closing if - stop
            else
            { //opening else - froward
              read_ir();
              // FORWARD
              if(right_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD && middle_sensor_state)
              { // opening if - forward
                Serial.println("going forward");
                get_current_status();             // get velocity
                Serial.println(speed_L);
                Serial.print("\t");
                Serial.print(speed_R);    
                advance(speed_R,speed_L);         // advance
                go_straight();                    // correct velocity
              } // closing if - forward
            } //closing else - froward 
          } // closing else - stop & forward
        } // closing else - left - stop - forward
      }// closing else - left - left - stop - forward
    }// closing else - right eft - left - stop - forward 
  }
  else if (count == -1)
  {
    int TimeMillis = 0;
    int time = millis();
    while(TimeMillis - time < 250)
    {                  //Time to turn towards bin
      Serial.println("---------------------Time to turn:  ");
      Serial.println(TimeMillis - time);
      back_off(speed_L,speed_R);
      TimeMillis = millis();
    }
    turn_90_r(speed_R,speed_R, millis());
    count = -3;
  }
  else
    stopp();

    Serial.println(speed_L);
    Serial.print("\t");
    Serial.println(speed_R); 
    
   previousMillis = currentMillis;
  //delay(500);
  /*  Serial.print("\n\tcur_wvel[0] " );
    Serial.print(cur_wvel[0]);
    Serial.print("\tcur_wvel[1] " );
    Serial.println(cur_wvel[1]);
  */
}
