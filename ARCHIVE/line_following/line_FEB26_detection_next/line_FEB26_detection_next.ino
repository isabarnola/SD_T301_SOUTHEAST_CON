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
int slow_R = 55;
int slow_L = 55;
#define plus_factor 6
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
#define TURN_TO_BIN    1
#define BACK           2
#define TURN_FORWARD   3
#define TURN_TO_LINE   4
#define time_turn      860
#define plus_dir 1
#define minus_dir 0

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
int pos = 0;
int white_lines = 0;
int state = 0;
int passed = 2;
int n = 1;


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

const int right_sensor_pin =10; // ORANGE
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

//-----------------------
// Sensor: FRONT Right 
//-----------------------
const int front_right_pin = 30;  // YELLOW
int front_right_sensor_state;


//-----------------------
// Sensor: FRONT left 
//-----------------------
const int front_left_pin = 31;  // GREEN
int front_left_sensor_state;

//-----------------------
// Sensor: low right 
//-----------------------
const int low_right_pin = 8;  // blue
int low_right_sensor_state;

//-----------------------
// Sensor: low left 
//-----------------------
const int low_left_pin = 7;  // green
int low_left_sensor_state;

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
  left_sensor_state         =  digitalRead(left_sensor_pin);
  right_sensor_state        =  digitalRead(right_sensor_pin);
  middle_sensor_state       =  digitalRead(middle_sensor_pin);
  left_arm_sensor_state     =  digitalRead(left_arm_sensor_pin);
  right_arm_sensor_state    =  digitalRead(right_arm_sensor_pin);
  front_right_sensor_state  =  digitalRead(front_right_pin);
  front_left_sensor_state   =  digitalRead(front_left_pin);
  low_right_sensor_state    =  digitalRead(low_right_pin);
  low_left_sensor_state     =  digitalRead(low_left_pin);
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

void go_straight2()
{
  int temp1 = abs(cur_wvel[1] -  cur_wvel[0]);
  
  if (temp1 >= 0.05 && temp1 <= 0.5 )
  {
    slow_L =slow_L;
    }
   else if (cur_wvel[1] > cur_wvel[0] )
      slow_L--;
    else if (cur_wvel[1] < cur_wvel[0])
      slow_L++;
}

void fix_turn_r_90_line(int right, int left)
{
  read_ir();
  while(left_sensor_state != BLACK && middle_sensor_state != BLACK && left_sensor_state != BLACK)
  {
    analogWrite (R_ME,right);
    analogWrite (L_ME,left);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    read_ir();
  }
}
void fix_turn_l_90_line(int right, int left)
{
  read_ir();
  while(left_sensor_state != BLACK && middle_sensor_state != BLACK && left_sensor_state != BLACK)
  {
    
    analogWrite (R_ME,right);
    analogWrite (L_ME,left);
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
 
  while(TimeMillis - time < time_turn)
   {                  //Time to turn towards bin
    Serial.println("---------------------Time to turn:  ");
    Serial.println(TimeMillis - time);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  //fix_turn_r_90_line(right,left);
}

void turn_90_l(int right, int left, int time)
{


  analogWrite (R_ME,right);
  analogWrite (L_ME,left);
  int TimeMillis = 0;
 
  while(TimeMillis - time < time_turn)
   {                  //Time to turn towards bin
    Serial.println("---------------------Time to turn:  ");
    Serial.println(TimeMillis - time);
    digitalWrite(R_M1,R_Backward);
    digitalWrite(L_M1,L_Forward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  
  //fix_turn_l_90_line(right,left);
}

void back_before_turn()
{
  long int TimeMillis = 0;
    
    long int time = millis();
    while(TimeMillis - time < 700)
    {                  //Time to turn towards bin
      Serial.println("---------------------backwardsn:  ");
      Serial.println(TimeMillis - time);
      back_off(speed_L,speed_R);
      TimeMillis = millis();
    }
}

void turn_left()
{
  
    back_before_turn();
    turn_90_l(speed_R,speed_R, millis());
    delay(10);

    
    Serial.println("looking for line");
    Serial.print(left_sensor_state);
    Serial.print("\t");
    Serial.print(middle_sensor_state);
    Serial.print("\t");
    Serial.print(right_sensor_state);
    Serial.print("\t");
    Serial.print(front_left_sensor_state);
    Serial.print("\t");
    Serial.println(front_right_sensor_state);   
    read_ir();

    int exit = 0;
    
    while (exit == 0)
    {
        read_ir();
        //L(B) M(?) R(W)
        if (front_left_sensor_state == BLACK && front_right_sensor_state == WHITE )
        {
          Serial.println("front_left_sensor_state == BLACK && front_right_sensor_state == WHITE  right correction");
          //back_off(0, turn_speed);
          back_off(turn_speed, 0); // worked
          //advance(0, turn_speed -7);
          
        }
        
        else if (front_left_sensor_state == WHITE && front_right_sensor_state == BLACK )
        {
          Serial.print(" front_left_sensor_state == WHITE && front_right_sensor_state == BLACK left correction");
          //back_off(turn_speed, 0);
          
          back_off(0, turn_speed); // worked
          //advance  (0,turn_speed -7);
        }
        else
        {
          Serial.println("aligned");
          stopp(); 
            exit = 1;
        }
    }

}

void back_to_line()
{   
    read_ir();
    while(left_sensor_state != WHITE && middle_sensor_state != WHITE && right_sensor_state != WHITE )
    { //Time to turn backwards bin
      get_current_status();
      back_off(speed_L,speed_R);
      go_straight();
      read_ir();
    }
}

void line_following(int lines, int dir)
{
  while(white_lines != lines)
  {
    read_ir();
      // LEFT
      //      | | |  
      //       L M  R
      //      | | |
      // <- L
      // L(W) M(W) R(B)
      if(left_sensor_state == WHITE      && middle_sensor_state == WHITE    && right_sensor_state == BLACK )
      {
        Serial.println("turning left L(W) M(W) R(B) ");
        // advance(speed_R, speed_L+ plus_factor);
         advance(speed_R + plus_factor , speed_L);
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
         //advance(speed_R +8, speed_L);
         advance(speed_R , speed_L + plus_factor);
       
      }
      else
      {// opening else - left - left - stop - forward
        read_ir();
        // LEFT
        //     ||||   
        //   L  M R
        //     ||||
        // L(B) M(W) R(W)
        // <-
          if(left_sensor_state == BLACK     && middle_sensor_state == WHITE    && right_sensor_state == WHITE )
        {// opening if - go right
          Serial.println("going left L(B) M(W) R(W)");
          //advance(speed_R +8, speed_L );
          //advance(speed_R , speed_L + plus_factor);
          advance(speed_R  + plus_factor , speed_L);
           
          
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
            Serial.println("turning right L(W) M(B) R(B) ");
             //advance(speed_R , speed_L +8);
             advance(speed_R + plus_factor, speed_L );
              
          } // closing if - going left
          else
          { // opening else - stop & forward
            read_ir();
            // STOP
            if(left_sensor_state == WHITE && middle_sensor_state == WHITE && right_sensor_state == WHITE)
            { // opening if - stop
              if (white_lines == lines)
              { // opening if - stop
                  Serial.println("stop          =       =       =     =         =       =       =       =       =     =");
                  stopp();
                  
                  state = TURN_TO_BIN;
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

  if (dir == plus_dir)
    pos = pos + white_lines;
  else if (dir == minus_dir)
    pos = pos - white_lines;

  white_lines = 0;
}




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


void loop() 
{
  
  currentMillis= millis();

  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();
    
  Serial.print("---------------------------------------------------------------White lines passed ");
  Serial.println(white_lines);
  Serial.print("STATE");
  Serial.println(state);
  
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
    //------------------------------------------
    // Navigation
    //------------------------------------------
    line_following(3, plus_dir);
    state = TURN_TO_BIN;
  }
  else if (state == TURN_TO_BIN)
  {
    turn_left();
    delay(2000); // delay to see turn
    state = TURN_FORWARD;
  }
  else if(state == TURN_FORWARD)
  {
       
    int exit = 0;
    int tempr = speed_R;
    int templ = speed_L;
    speed_R =  speed_R - 10;
    speed_L =  speed_L - 10;
    while (exit == 0)
    {
        read_ir();
        if ((left_arm_sensor_state == WHITE && right_arm_sensor_state == WHITE ) && (low_right_sensor_state == WHITE || low_left_sensor_state == WHITE ) || 
            (low_right_sensor_state == WHITE || low_left_sensor_state == WHITE ))
        {
          stopp();
          exit = 1;
        }
        else if (left_arm_sensor_state == WHITE && right_arm_sensor_state == BLACK)
        {
          Serial.print("left arm");
          turn_R_line(turn_speed+5);
        }
        else if(left_arm_sensor_state == BLACK && right_arm_sensor_state == WHITE)
        {
          turn_L_line(turn_speed+6);      
          
        }
        else
        {
          Serial.println("Forward"); 
          get_current_status();
          go_straight();
          advance(speed_R,speed_L); // move forward
         }
    }
    speed_R  = tempr;
    speed_L =  templ;
    //delay(500);
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
      state = TURN_TO_LINE;
  }
  else if (state == TURN_TO_LINE)
  {
    turn_90_r(speed_R,speed_R, millis());
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
