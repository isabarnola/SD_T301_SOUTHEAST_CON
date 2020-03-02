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
#define GO_TO_BIN      3
#define TURN_TO_LINE   4
#define LF2            6
#define R90LINE        5
#define time_turn      860
#define t_back_trb    700
#define t_back_tlb    1100
#define time_back_turn_bin_l 700
#define time_turn_r    1400
#define time_turn_r_l  1200
#define time_turn_l_l  1200
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


#include "FUNCTIONS.h"



void turn_left_bin(int turntime)
{
    turn_90_l(speed_R,speed_R, millis(), turntime);   
    delay(100);
    aligned_bin(millis(),200);  
}

void turn_right_line(int turntime)
{
   analogWrite (R_ME,speed_R);
  analogWrite (L_ME,speed_L);
  int TimeMillis = 0;
  int correct = 0;
 int time1 = millis();
  while(TimeMillis - time1 < turntime)
   {                  //Time to turn towards bin
    Serial.println("turn_right_line ---------------------Time to turn:  ");
    Serial.println(TimeMillis - time1);
    digitalWrite(R_M1,R_Backward);
    digitalWrite(L_M1,R_Forward);
    TimeMillis = millis();
    read_ir();
  }
    analogWrite (R_ME,0);
    analogWrite (L_ME, 0);
}

void turn_left_line(int turntime)
{
  Serial.println("FUNCTION turn_left_line");
  analogWrite (R_ME,speed_R);
  analogWrite (L_ME,speed_L);
  int TimeMillis = 0;
  int correct = 0;
 int time1 = millis();
  while(TimeMillis - time1 < turntime)
   {                  //Time to turn towards bin
    Serial.println("turn_right_line ---------------------Time to turn:  ");
    Serial.println(TimeMillis - time1);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    TimeMillis = millis();
    read_ir();
    if (front_left_sensor_state == WHITE)
    {
      Serial.println("correct = 1");
      correct = correct+1;
      TimeMillis = turntime;
    }
  }
  
/*
  if(correct == 2)
  {
    read_ir();
    while (front_left_sensor_state == BLACK)
    {
      digitalWrite(R_M1,R_Backward);
       digitalWrite(L_M1,L_Forward);
       read_ir();
     }
     analogWrite (R_ME,0);
    analogWrite (L_ME, 0);

   }
   */
    analogWrite (R_ME,0);
    analogWrite (L_ME, 0);
}

void turn_right_bin(int turntime)
{
    Serial.print("turn_right_bin(");
    turn_90_r(speed_R,speed_R, millis(), turntime);  
    delay(100);
    back_to_line_front();
    aligned_bin_right(millis(),200); 
}

void aligned_bin(int time1, int correct_time)
{
  
    //int correct_time = 200;
    //int time1 = millis();
    Serial.println("==========alignee=======");
     read_ir();

    int exit = 0;
    int TimeMillis = 0;
    int invert = 0;
    while (exit == 0)
    {

        Serial.print(front_left_sensor_state);
        Serial.print("\t");
        Serial.print(front_right_sensor_state);
        Serial.print("\t");
        Serial.println(TimeMillis);
        
        if (TimeMillis - time1 < correct_time)
          invert =1;
        
        read_ir();

        //L(B) M(?) R(W)
        if (front_left_sensor_state == BLACK && front_right_sensor_state == WHITE )
        {
          Serial.println("front_left_sensor_state == BLACK && front_right_sensor_state == WHITE");
          //back_off(0, turn_speed);
          if (invert == 0)
          {
             advance(turn_speed+7, 0);
            Serial.println("advance(0, turn_speed");
          }
          //advance(0, turn_speed -7);
          else
          {
            back_off(turn_speed+7, 0); // worked
            Serial.println(" back_off(0,turn_speed);");
          }
          
          
        }
        
        else if (front_left_sensor_state == WHITE && front_right_sensor_state == BLACK )
        {
          Serial.print(" front_left_sensor_state == WHITE && front_right_sensor_state == BLACK left correction");
          //back_off(turn_speed, 0);
          if (invert == 0)
          {
            advance(0, turn_speed+7); // worked = turn_l_line
             Serial.println(" advance(turn_speed, 0);");
          }
          //advance  (0,turn_speed -7);
          else
          {
            back_off(0,turn_speed+7); // worked 
            Serial.println("back_off(turn_speed, 0);");
          }
          
        }
        else if(front_left_sensor_state == BLACK && front_right_sensor_state == BLACK)
        {
         if (invert == 0)
         {
          back_off (turn_speed, turn_speed);
          } 
          else
          {
            advance(speed_R,speed_L);
          }
         }
        else
        {
          Serial.println("aligned");
          stopp(); 
            exit = 1;
        }
        TimeMillis = millis();
    }
}


void aligned_bin_right(int time1, int correct_time)
{
   Serial.println("FUNCTION aligned_bin_right");
    //int correct_time = 200;
    //int time1 = millis();
    Serial.println("==========aligned=======");
     read_ir();

    int exit = 0;
    int TimeMillis = 0;
    int invert = 0;
    while (exit == 0)
    {

        Serial.print(front_left_sensor_state);
        Serial.print("\t");
        Serial.print(front_right_sensor_state);
        Serial.print("\t");
        Serial.println(TimeMillis);
        
        if (TimeMillis - time1 < correct_time)
          invert =1;
        
        read_ir();

        //L(B) M(?) R(W)
        if (front_left_sensor_state == BLACK && front_right_sensor_state == WHITE )
        {
          Serial.println("front_left_sensor_state == BLACK && front_right_sensor_state == WHITE");
          //back_off(0, turn_speed);
          if (invert == 0)
          {
             //advance(turn_speed+7, 0);
             advance(0, turn_speed+7);
            Serial.println("advance(0, turn_speed+7)");
          }
          //advance(0, turn_speed -7);
          else
          {
            //back_off(turn_speed+7, 0); // worked
            back_off(0, turn_speed+7);
            Serial.println(" back_off(0, turn_speed+7);");
          }
          
          
        }
        
        else if (front_left_sensor_state == WHITE && front_right_sensor_state == BLACK )
        {
          Serial.print(" front_left_sensor_state == WHITE && front_right_sensor_state == BLACK left correction");
          //back_off(turn_speed, 0);
          if (invert == 0)
          {
            //advance(0, turn_speed+7); // worked = turn_l_line
            advance(turn_speed+7, 0);
             Serial.println(" advance(turn_speed+7, 0)");
          }
          //advance  (0,turn_speed -7);
          else
          {
            //back_off(0,turn_speed+7); // worked
            back_off(turn_speed+7, 0) ;
            Serial.println("back_off(turn_speed+7, 0) ;");
          }
          
        }
        else if(front_left_sensor_state == BLACK && front_right_sensor_state == BLACK)
        {
         if (invert == 0)
         {
          back_off (turn_speed, turn_speed);
          } 
          else
          {
            advance(speed_R,speed_L);
          }
         }
        else
        {
          Serial.println("aligned");
          stopp(); 
            exit = 1;
        }
        TimeMillis = millis();
    }
}



void back_to_line()
{   
  Serial.println("BACK TO LINE");

   int r = speed_R;
   int l = speed_L;
   speed_R = speed_R - 20;
   speed_L = speed_L - 20;
    read_ir();
     while(left_sensor_state == BLACK || right_sensor_state == BLACK)
    { //Time to turn backwards bin
      get_current_status();
      back_off(speed_R,speed_L);
      go_straight();
      read_ir();

      Serial.print("SPEED");
      Serial.print(speed_L);
      Serial.print("\t");
      Serial.println(speed_R);
    }
    speed_R = r;
    speed_L= l;
   
    /*while(left_sensor_state != WHITE && middle_sensor_state != WHITE && right_sensor_state != WHITE )
    { //Time to turn backwards bin
      get_current_status();
      read_ir();
      back_off(speed_L,speed_R);
      go_straight();
     // read_ir();
    }*/
}

void back_to_line_front()
{
   Serial.println("BACK TO LINE");

   int r = speed_R;
   int l = speed_L;
   speed_R = speed_R - 20;
   speed_L = speed_L - 20;
    read_ir();
     while(front_left_sensor_state == BLACK || front_right_sensor_state == BLACK)
    { //Time to turn backwards bin
      get_current_status();
      back_off(speed_R,speed_L);
      go_straight();
      read_ir();

      Serial.print("SPEED");
      Serial.print(speed_L);
      Serial.print("\t");
      Serial.println(speed_R);
    }
    speed_R = r;
    speed_L= l;
  
  }

void print_state()
{
  if (state == STOP)
    Serial.println("STATE = STOP ###################");
   else if (state == LINE_FOLLOWING)
   {
    Serial.println("STATE = LINE_FOLLOWING ###################");
    }
    else if (state == TURN_TO_BIN)
    {
      Serial.println("STATE = TURN_TO_BIN ###################");
      }
     else if(state == GO_TO_BIN)
     {
       Serial.println("STATE = GO_TO_BIN ###################");
      }
      else if(state == BACK)
      {
        Serial.println("STATE = BACK ###################");
        }
        else if(state == TURN_TO_LINE)
        {
          Serial.println("STATE = TURN_TO_LINE ###################");
          }
          else if (state == LF2)
          {
            Serial.println("STATE = LF2 ###################");
            }
  }

void go_to_bin()
{
  Serial.println("GO TO BIN FUNCTION");
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
          Serial.print(left_arm_sensor_state);
           Serial.print("\t");
          Serial.print(right_arm_sensor_state);
           Serial.print("\t");
          Serial.print(low_left_sensor_state);
           Serial.print("\t");
          Serial.println(low_right_sensor_state);
          
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
  }

void back_off_time(int time_t)
{
   Serial.println("FUNCTION back_off_time");
  long int TimeMillis = 0;
 long int time1 = micros();
  while(TimeMillis - time1 < time_t)
   {      
      Serial.print("Timemillis ");
    Serial.println(TimeMillis);
    Serial.print("Time1 ");
    Serial.println(time1);
     Serial.println(TimeMillis - time1 );
     Serial.println("back off to turn");
     back_off(speed_L-15,speed_R-15);            
    TimeMillis = micros();
    
  }
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
  /*
 Serial.print(left_arm_sensor_state);
 Serial.print("\t");
 Serial.print(left_sensor_state);
 Serial.print("\t");
 Serial.print(middle_sensor_state);
 Serial.print("\t");
 Serial.print(right_sensor_state);
 Serial.print("\t");
 Serial.println(right_arm_sensor_state);
   */
  if (state == LINE_FOLLOWING)
  {
    Serial.println("LINE FOLLOWING");
      print_state();
    //------------------------------------------
    // Navigation
    //------------------------------------------
    line_following(2, plus_dir);
    state = TURN_TO_BIN;
    Serial.println("turn TO BIN");
  }
  else if (state == TURN_TO_BIN)
  {
    Serial.println("turn TO BIN");
    print_state();
    delay(100);
    back_before_turn(t_back_tlb );
    delay(200);
    turn_right_bin(time_turn_r);

    
    delay(2000); // delay to see turn
    state = GO_TO_BIN;
    Serial.println("to GO TO BIN");
  }
  else if(state == GO_TO_BIN)
  {
    Serial.println("GO TO BIN");
      print_state();
    go_to_bin();
    //delay(500);
    state = BACK;
    Serial.println("to BACK");
  } 
  else if (state == BACK)
  {
    Serial.println("BACK");
      print_state();
      Serial.println("back_to_line====================");
      //ba-ck_to_line();
        
      back_to_line();
      stopp();
      delay(300);
      state = TURN_TO_LINE;//R90LINE;
      Serial.println("to TURN_TO_LINE");
  }
  else if (state == TURN_TO_LINE)
  {
    Serial.println("TURN_TO_LINE");
      print_state();
    delay(1000);
    back_off_time(600);
    delay(1000);
    turn_left_line(time_turn_l_l);
    state = LF2; 
    delay(300);
    Serial.println("to LF2");
  }
  else if(state == LF2)
  {
    Serial.print("LF2");
    print_state();
    line_following(2, plus_dir);
    state = STOP;
     Serial.println("to STOP");
    }
  else if (state == STOP)
  {
    print_state();
    Serial.println("STOP");
    stopp();
    state = STOP;
    
  }
/*
    Serial.println(speed_L);
    Serial.print("\t");
    Serial.println(speed_R); 

    Serial.print("state");
    Serial.println(state);
  */
   previousMillis = currentMillis;
  //delay(500);
  /*  Serial.print("\n\tcur_wvel[0] " );
    Serial.print(cur_wvel[0]);
    Serial.print("\tcur_wvel[1] " );
    Serial.println(cur_wvel[1]);
  */
}
