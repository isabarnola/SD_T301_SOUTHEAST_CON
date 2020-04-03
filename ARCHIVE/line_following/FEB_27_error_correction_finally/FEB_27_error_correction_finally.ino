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


#include "FUNCTIONS.h"



void turn_left(int turntime)
{
  
    back_before_turn();
    turn_90_l(speed_R,speed_R, millis(), turntime);
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
    
    //aligned(millis(),200);  
}

void aligned(int time, int correct_time)
{
  read_ir();

    int exit = 0;
    int TimeMillis = 0;
    int invert = 0;
    while (exit == 0)
    {
        if (TimeMillis - time < correct_time)
          invert =1;

        read_ir();

        //L(B) M(?) R(W)
        if (front_left_sensor_state == BLACK && front_right_sensor_state == WHITE )
        {
          Serial.println("front_left_sensor_state == BLACK && front_right_sensor_state == WHITE  right correction");
          //back_off(0, turn_speed);
          if (invert == 0)
            advance(0, turn_speed); // worked = turn_l_line
          //advance(0, turn_speed -7);
          else
          {
            back_off(0,turn_speed); // worked
          }
          
          
        }
        
        else if (front_left_sensor_state == WHITE && front_right_sensor_state == BLACK )
        {
          Serial.print(" front_left_sensor_state == WHITE && front_right_sensor_state == BLACK left correction");
          //back_off(turn_speed, 0);
          if (invert == 0)
            advance(turn_speed, 0); // worked
          //advance  (0,turn_speed -7);
          else
          {
            back_off(turn_speed, 0); // worked
          }
          
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
    line_following(2, plus_dir);
    state = TURN_TO_BIN;
  }
  else if (state == TURN_TO_BIN)
  {
    turn_left(time_turn);

    int correct_time = 200;
    int time1 = millis();
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
             advance(turn_speed, 0);
            Serial.println("advance(0, turn_speed");
          }
          //advance(0, turn_speed -7);
          else
          {
            back_off(turn_speed, 0); // worked
            Serial.println(" back_off(0,turn_speed);");
          }
          
          
        }
        
        else if (front_left_sensor_state == WHITE && front_right_sensor_state == BLACK )
        {
          Serial.print(" front_left_sensor_state == WHITE && front_right_sensor_state == BLACK left correction");
          //back_off(turn_speed, 0);
          if (invert == 0)
          {
            advance(0, turn_speed); // worked = turn_l_line
             Serial.println(" advance(turn_speed, 0);");
          }
          //advance  (0,turn_speed -7);
          else
          {
            back_off(0,turn_speed); // worked 
            Serial.println("back_off(turn_speed, 0);");
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
      state = STOP;
  }
  else if (state == TURN_TO_LINE)
  {
    turn_90_r(speed_R,speed_R, millis(),time_turn);
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
