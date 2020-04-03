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
#define STOP          -1
#define LINE_FOLLOWING 0
#define TURN           1
#define BACK           2

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
int e_turn = 0;
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
// Sensor: Right
//-----------------------

const int right_sensor_pin = 10; // ORANGE
int right_sensor_state;


//-----------------------
// Sensor: Left
//-----------------------

const int left_sensor_pin  = 11; // BLUE
int left_sensor_state;


//-----------------------
// Sensor: Middle
//-----------------------
const int middle_sensor_pin = 12; // WHITE
int middle_sensor_state;

//-----------------------
// Sensor: Left arm
//-----------------------

const int left_arm_sensor_pin  = 13; // GREEN
int left_arm_sensor_state;

//-----------------------
// Sensor: Right arm
//-----------------------
const int right_arm_sensor_pin = 9; // BLUE
int right_arm_sensor_state;


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
  left_sensor_state   =  digitalRead(left_sensor_pin);
  right_sensor_state  = digitalRead(right_sensor_pin);
  middle_sensor_state  =  digitalRead(middle_sensor_pin);
  left_arm_sensor_state  =  digitalRead(left_arm_sensor_pin);
  right_arm_sensor_state  =  digitalRead(right_arm_sensor_pin);
   
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

void fix_turn_r_90_line()
{
  read_ir();
  while(left_sensor_state != BLACK && middle_sensor_state != BLACK && left_sensor_state != BLACK)
  {
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    read_ir();
  }
}
void fix_turn_l_90_line()
{
  read_ir();
  while(left_sensor_state != BLACK && middle_sensor_state != BLACK && left_sensor_state != BLACK)
  {
    digitalWrite(R_M1,R_Backward);
    digitalWrite(L_M1,L_Forward);
    read_ir();
  }
}

void turn_90_r(int right, int left, int time)
{
  analogWrite (R_ME,right);
  analogWrite (L_ME,left);
  int TimeMillis = 0;
 
  while(TimeMillis - time < 875)
   {                  //Time to turn towards bin
    Serial.println("---------------------Time to turn:  ");
    Serial.println(TimeMillis - time);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  fix_turn_r_90_line();
}

void turn_90_l(int right, int left, int time)
{
  analogWrite (R_ME,right);
  analogWrite (L_ME,left);
  int TimeMillis = 0;
 
  while(TimeMillis - time < 875)
   {                  //Time to turn towards bin
    Serial.println("---------------------Time to turn:  ");
    Serial.println(TimeMillis - time);
    digitalWrite(R_M1,R_Backward);
    digitalWrite(L_M1,L_Forward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  fix_turn_l_90_line();
}

void back_to_line()
{   
    read_ir();
    while(left_sensor_state == BLACK && right_sensor_state == BLACK  )
    { //Time to turn backwards bin
      get_current_status();
      back_off(speed_L,speed_R);
      go_straight();
      read_ir();
    }
}

int state = 0;

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
  pinMode(left_sensor_pin,    INPUT);  // Left
  pinMode(middle_sensor_pin,  INPUT);  // Middle
  pinMode(left_arm_sensor_pin, INPUT); 
  pinMode(right_arm_sensor_pin, INPUT);
  
  //----------------------
  // Encoders
  //----------------------
  encoder_init();

  Serial.begin(9600); 

}

int passed = 2;
int n = 3;
int white_lines = 0;

void loop() 
{
  
  currentMillis= millis();

  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();
    
  Serial.print("---------------------------------------------------------------White lines passed ");
  Serial.println(white_lines);
  
 Serial.print(left_arm_sensor_state);
 Serial.print("\t");
 Serial.print(left_sensor_state);
 Serial.print("\t");
 Serial.print(middle_sensor_state);
 Serial.print("\t");
 Serial.print(right_sensor_state);
 Serial.print("\t");
 Serial.println(right_arm_sensor_state);
   
  if (state == LINE_FOLLOWING)
  {
    //Serial.print("Navigation");
    //------------------------------------------
    // Navigation
    //------------------------------------------

    // LEFT
    //      | | |  
    //       L M  R
    //      | | |
    // <- L
    // L(W) M(W) R(B)
    if(left_sensor_state == WHITE      && middle_sensor_state == WHITE    && right_sensor_state == BLACK )
    {
      Serial.println("turning rleft L(W) M(W) R(B) ");
      advance(speed_R, speed_L+8);
     
      
    }
    else
    {  // opening else - right eft - left - stop - forward 
        read_ir();
        //      | | |  
        //       R
        //     M  | |
        //   L  | | |  
        //-> R
        // L(B) M(B) R(W)
      if (left_sensor_state == BLACK       && middle_sensor_state == BLACK    &&  right_sensor_state == WHITE )
      {
        Serial.println("going right L(B) M(B) R(W)");
         advance(speed_R +8, speed_L);
       
      }
      else
      {// opening else - left - left - stop - forward
        read_ir();
        // LEFT
        //     ||||   
        //   L  M R
        //     ||||
        // L(B) M(W) R(W)
        // ->
          if(left_sensor_state == BLACK     && middle_sensor_state == WHITE    && right_sensor_state == WHITE )
        {// opening if - go right
          Serial.println("going right");
          advance(speed_R +8, speed_L );
           
          
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
          if (left_sensor_state == WHITE && middle_sensor_state == BLACK && right_sensor_state == BLACK  )
          { // opening if - going left
            Serial.println("turning right L(1) M(0) R(0) ");
             advance(speed_R , speed_L +8);
              
          } // closing if - going left
          else
          { // opening else - stop & forward
            read_ir();
            // STOP
            if(left_sensor_state == WHITE && middle_sensor_state == WHITE && right_sensor_state == WHITE)
            { // opening if - stop
              if (white_lines == n)
              { // opening if - stop
                  Serial.println("stop          =       =       =     =         =       =       =       =       =     =");
                  stopp();
                  
                  state = TURN;
              } // closing if - stop
              else
              { // opening else - pass white line
                while(left_sensor_state == WHITE && middle_sensor_state == WHITE && right_sensor_state == WHITE) 
                { // opening while loop - pass white line
                      read_ir();
                    Serial.print("|||||||||||||||| PASSING WHITE||||||||||||||||||  ");
                    Serial.println(white_lines);
                    get_current_status();     // get velocity
                 
                    advance(speed_R,speed_L); // move forward
                    go_straight();            // correct velocity
                     e_turn =0;
                } // closing while loop - pass white line
                white_lines++;
               
                
               // state++;
              } // close else - pass white line
            } // closing if - stop
            else
            { //opening else - froward
              read_ir();
              // FORWARD
              if(left_sensor_state == BLACK && middle_sensor_state == WHITE && right_sensor_state == BLACK)  // ASK ISA 
              { // opening if - forward
                Serial.println("going forward");
                get_current_status();             // get velocity
                Serial.println(speed_L);
                Serial.print("\t");
                Serial.print(speed_R);    
                advance(speed_R,speed_L);         // advance
                go_straight();                    // correct velocity
                    e_turn =0;
              } // closing if - forward
            } //closing else - froward 
          } // closing else - stop & forward
        } // closing else - left - stop - forward
      }// closing else - left - left - stop - forward
    }// closing else - right eft - left - stop - forward 
  }
  else if (state == TURN)
  {
    int TimeMillis = 0;
    int TimeMillis2 = 0;
    int time = millis();
    while(TimeMillis - time < 700)
    {                  //Time to turn towards bin
      Serial.println("---------------------backwardsn:  ");
      Serial.println(TimeMillis - time);
      back_off(speed_L,speed_R);
      TimeMillis = millis();
    }
     
    turn_90_l(speed_R,speed_R, millis());
    delay(1000); // delay to see turn
    int time2 = millis();
    Serial.println("Forward");
    while( TimeMillis2 - time2 < 500)
    {
      Serial.println("eyeballing forward");
      get_current_status();
      go_straight();
      advance(speed_R,speed_L); // move forward
      
      TimeMillis2 = millis();
    }
    Serial.println("sensors");
    int exit = 0;
    while (exit == 0)
    {
        read_ir();
        if (left_arm_sensor_state == WHITE && right_arm_sensor_state == WHITE )
        {
          stopp();
          exit = 1;
        }
         /* while( TimeMillis2 - time2 < 300)
          {
            get_current_status();
            go_straight();
            advance(speed_R,speed_L); // move forward
            
            TimeMillis2 = millis();
          }*/
          if (left_arm_sensor_state == WHITE && right_arm_sensor_state == BLACK)
          {
            Serial.print("left arm");
            turn_R_line(turn_speed);
          }
          else if(left_arm_sensor_state == BLACK && right_arm_sensor_state == WHITE)
          {
            turn_L_line(turn_speed);      
            
          }
    }
    
    state = BACK;
  } 
  else if (state == BACK)
  {
      Serial.println("back_to_line====================");
      //back_to_line();
        
    read_ir();
    while(left_sensor_state == BLACK && right_sensor_state == BLACK  )
    { //Time to turn backwards bin
      get_current_status();
      back_off(speed_L,speed_R);
      //go_straight();
      read_ir();
      
    Serial.println(speed_L);
    Serial.print("\t");
    Serial.println(speed_R);
    }
      state = STOP;
  }
  else
  {
    stopp();
  }

    Serial.println(speed_L);
    Serial.print("\t");
    Serial.println(speed_R); 

    Serial.print("state");
    Serial.println(state);
   previousMillis = currentMillis;
  //delay(500);
  /*  Serial.print("\n\tcur_wvel[0] " );
    Serial.print(cur_wvel[0]);
    Serial.print("\tcur_wvel[1] " );
    Serial.println(cur_wvel[1]);
  */
}
