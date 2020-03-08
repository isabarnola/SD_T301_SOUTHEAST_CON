/*
  FILE :  Stacking_challenge_7digits
  DESCRIPTION: 1st Challenge of SoutheastCon Hardware competition 2020
  AUTHOR : ISABEL BARNOLA
  DATE: 03/03
*/

// ================================================================
// MOTORS
// ================================================================
#define L_Forward  HIGH //LOW
#define R_Forward  LOW  //HIGH
#define L_Backward LOW  //HIGH
#define R_Backward HIGH //LOW
#define plus_factor 4
#define turn_speed  50
#define turn_speed1 600
int speed_R  = 57;//54;//55;//65;//45; //52; //49; //68; //49;//49;//68;//95;//85;// MAX = 255
int speed_L = 52;//0;//69;//49;        //68;//49;  //105;//95; // MAX = 255
int slow_R = 55;
int slow_L = 55;

//-----------------------
// Motor : Right
//-----------------------
// M1 PWM
//ORANGE
int R_ME  = 3; //2;//3;//2;//6;   //Enable Pin of the Right Motor (must be PWM)
// M1 EN 
// YELLOW
int R_M1 = 23;//22;//23;//22; //51;    //Control Pin

//-----------------------
// Motor : Left
//-----------------------
// GREEN
int L_ME  = 2;//3;//2;//3;//4;   //Enable Pin of the Left Motor (must be PWM)
// BLUE
int L_M1 = 22;//23; //22;//23;//50;

int dir = 0;
int cnt = 0;

long previousMillis = 0;
long currentMillis = 0;
int interval = 1000;

// ================================================================
// ENCODERS
// ================================================================
#include "mrobot.h" //Provided by Dr. Chuy


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
const int right_arm_sensor_pin = 28; // BLUE
int right_arm_sensor_state;

//-----------------------
// Sensor: FRONT Right 
//-----------------------
const int front_right_pin = 29;  // GREEN
int front_right_sensor_state;

//-----------------------
// Sensor: FRONT left 
//-----------------------
const int front_left_pin = 30;  // YELLOW
int front_left_sensor_state;

//-----------------------
// Sensor: low right 
//-----------------------
const int low_right_pin = 31;  // blue
int low_right_sensor_state;

const int led = 32;

//-----------------------
// Sensor: low left 
//-----------------------
const int low_left_pin = 7;  // green
int low_left_sensor_state;


// ================================================================
//TURN_BIN
// ================================================================
//Left
#define t_back_tlb           700  // time to back up on left
#define time_turn_lb         860  // time to turn on left

//Right
#define t_back_trb           900//1000 // time to back up on right
#define time_turn_rb         1000//1200 // time to turn on right

// ================================================================
//TURN_LINE
// ================================================================
//Left
#define t_back_tll           300  // time to back up on left
#define time_turn_l_l        1100 // time to turn on left
// Right
#define t_back_tlr           600  // time to back up on right
#define time_turn_r_l        1200 // time to turn on right

int e_turn = 0;
int pos = 0;   // used to determine robot's current position 

// ================================================================
// STATES
// ================================================================
#define STOP             0
#define LINE_FOLLOWING   1
#define TURN_RIGHT_BIN   2
#define TURN_LEFT_BIN    3
#define TURN_RIGHT_LINE  4
#define TURN_LEFT_LINE   5
#define BACK_TO_LINE     6
#define STACK            7
#define TURN_180         8
#define GO_TO_BIN        9 

const int num_states_to_do = 11;
int states_to_do[num_states_to_do] = {
                                          LINE_FOLLOWING,
                                          TURN_RIGHT_BIN,
                                          GO_TO_BIN,
                                          BACK_TO_LINE,
                                          TURN_LEFT_LINE,
                                          LINE_FOLLOWING,
                                          TURN_LEFT_BIN,
                                          GO_TO_BIN,
                                          BACK_TO_LINE,
                                          TURN_RIGHT_LINE,
                                          STOP};
int state = LINE_FOLLOWING;
int itr_s = -1;           // used to iterate through array of states
#define RIGHT 10
#define LEFT  11
int last_dir = 0;

// ================================================================
//LINE_FOLLOWING
// ================================================================
#define BLACK HIGH
#define WHITE LOW
#define plus_dir  1
#define minus_dir -1
const int bins_to_go_LF = 5; // the total number of bins to go with LF
int white_lines = 0;
int lines_to_do[bins_to_go_LF] = {3,2,1,1,1};
//int direc_to_do[bins_to_go_LF] = {plus_dir};
int do_lf = 0;

// ================================================================
//FUNCTIONS
// ================================================================
#include "FUNCTIONS.h"

// ================================================================
// SETUP
// ================================================================
void setup() 
{ // opening setup

  //-----------------------
  // Motors
  //-----------------------
  pinMode(R_M1, OUTPUT);  // Right
  pinMode(L_M1, OUTPUT);  // Left

  //-----------------------
  // IR Sensors
  //-----------------------
  pinMode(right_sensor_pin,   INPUT);   // Right
  pinMode(left_sensor_pin,    INPUT);   // Left
  pinMode(middle_sensor_pin,  INPUT);   // Middle
  pinMode(left_arm_sensor_pin, INPUT);  // left arm
  pinMode(right_arm_sensor_pin, INPUT); // right arm
  pinMode(low_left_pin, INPUT);         // lower left
  pinMode(low_right_pin, INPUT);        // lower right
  pinMode(front_left_pin,INPUT);        // front left
  pinMode(front_right_pin,INPUT);       // front right
  pinMode(led,OUTPUT);
  
  //----------------------
  // Encoders
  //----------------------
  encoder_init();

  Serial.begin(9600);  // For serial print (debugging)

}
  // closing setup

  int ii = 0;
// ================================================================
// LOOP
// ================================================================
void loop() 
{ // opening loop

  //-------------------------------------
  // STATE: Update
  //-------------------------------------
  
  state = set_state();
  Serial.print("state: ");
  Serial.println(state);
  Serial.print("whitelines");
  Serial.println(white_lines);
  

  Serial.println(ii);
  //-------------------------------------
  // IR Sensors: READ
  //-------------------------------------
   read_ir(); 

  //-------------------------------------
  // FSM
  //-------------------------------------
  
  switch(state)
  {//opening switch
    
    //-----------------------
    //LINE_FOLLOWING
    //-----------------------
    case LINE_FOLLOWING:
      { //opening LINE_FOLLOWING
        
         print_state(); // prints the current state to serial port
         // Line following - parameters set on top
         line_following(lines_to_do[do_lf],plus_dir); 
         do_lf = do_lf + 1;
      } 
        //closing LINE_FOLLOWING
      break;

    //-----------------------
    // TURN_RIGHT_BIN
    //-----------------------
    case TURN_RIGHT_BIN:
      { //opening TURN_RIGHT_BIN
        print_state(); // prints the current state to serial port
        delay(100);
        back_before_turn(t_back_trb);
        delay(200);
        turn_left_bin(time_turn_rb);
        last_dir = RIGHT;
      } 
        //closing TURN_RIGHT_BIN
      break;

    //-----------------------
    //TURN_LEFT_BIN
    //-----------------------
    case TURN_LEFT_BIN:
      { //opening TURN_LEFT_BIN
        print_state(); // prints the current state to serial port
        delay(100);
        back_before_turn(t_back_tlb);  
        turn_right_bin(time_turn_lb);
        last_dir = LEFT;   
      } 
        //closing TURN_LEFT_BIN
      break;

    //-----------------------
    //TURN_RIGHT_LINE
    //-----------------------
    case TURN_RIGHT_LINE:
      { //opening TURN_RIGHT_LINE
        print_state(); // prints the current state to serial port
        delay(100);
        back_off_time(t_back_tlr);
        delay(200);
        turn_left_line(time_turn_r_l);
      } 
        //closing TURN_RIGHT_LINE
      break;

    //-----------------------
    //TURN_LEFT_LINE
    //-----------------------
    case TURN_LEFT_LINE:
      { //opening TURN_LEFT_LINE
        print_state(); // prints the current state to serial port
        delay(100);
         advance_before_tll(400);
         stopp();
         delay(300);
        turn_right_line(time_turn_l_l);
      } 
        //closing TURN_LEFT_LINE
      break;

    //-----------------------
    //TURN_180 
    //-----------------------
    case TURN_180:
      { //opening TURN_180
        print_state(); // prints the current state to serial port
        if(last_dir == LEFT)
        { // opening when last was left
          // 90 degrees right - to the line
          delay(100);
          back_off_time(t_back_tlr);
          delay(100);
          turn_right_line(time_turn_r_l);
          // 90 degrees right  - to the bin
          delay(100);
          back_before_turn(t_back_trb);
          delay(200);
          turn_right_bin(time_turn_rb);;
        }
          // closing when last was left
        else 
        { // opening when last was right 
           // 90 degrees left - to the line
           delay(100);
           back_off_time(t_back_tlr);
           delay(100);
           turn_right_line(time_turn_r_l);
           // 90 degrees left  - to the bin
           delay(100);
           back_before_turn(t_back_tlb);  
           turn_left_bin(time_turn_lb);
        }
          // closing when last was right
      } 
        //closing TURN_180
      break;

    //-----------------------
    // GO_TO_BIN
    //-----------------------
    case GO_TO_BIN:
      { //opening GO_TO_BIN
        print_state(); // prints the current state to serial port
         go_to_bin();
      } 
        //closing GO_TO_BIN
      break;

    //-----------------------
    // BACK_TO_LINE
    //-----------------------
    case BACK_TO_LINE:
      { //opening BACK_TO_LINE
        print_state(); // prints the current state to serial port
        back_to_line();
        stopp();
        delay(300);
      } 
        //closing BACK_TO_LINE
      break;

    //-----------------------
    //STACK
    //-----------------------
    case STACK:
      { //opening STACK
        print_state(); // prints the current state to serial port
      } 
        //closing STACK
      break;

    //-----------------------
    //STOP
    //-----------------------
    case STOP:
      { //opening STOP
        print_state(); // prints the current state to serial port
        stopp();
      } 
        //closing STOP
      break;

    //-----------------------
    // default
    //-----------------------
    default:
      { //opening default
        stopp();
      }
        //closing default
      break;
      
  }//closing switch
 
}
  // closing loop
