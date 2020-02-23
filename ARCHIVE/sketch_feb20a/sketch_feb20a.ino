// ================================================================
// H BRIDGE 
// ================================================================
#define ENCODEROUTPUT 4741.44
#define MIN 80
#define motor_speed 100 // MAX = 255
int motor_speed_R  = 80;//95;//85;// MAX = 255
int motor_speed_L = 60;//105;//95; // MAX = 255
#define turn_speed  50
#define turn_speed1 50
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
int ENC_RB = 20;
int ENC_LA = 3;
int ENC_LB = 21;
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

//-----------------------
// Robot to Right Bin
//-----------------------
int TimeMillis = 0;
int Turn_time;
int Right_Bin = 1;
int go_straight = 1;
int advance_to_bin = 0;
//-----------------------
// Robot to Left Bin
//-----------------------
int Left_Bin = 0;

//---------------------
// Robot back to Line
//----------------------
int back_off_from_bin = 0;
int keep_going = 0;
char from_side;
int swing_back = 0;

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
void go_to_bin(char a,char b, int c)          //Move forward
{
   analogWrite (R_ME,a);
  analogWrite (L_ME,b);
  //int TimeMillis = millis();
 
  while(TimeMillis - c < 500) {
    Serial.println("---------------------Time to turn:  ");
    Serial.print(TimeMillis - c);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Forward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  
}
void read_ir()
{
  left_sensor_state   =  analogRead(left_sensor_pin);
  right_sensor_state  =  analogRead(right_sensor_pin);
  middle_sensor_state  =  analogRead(middle_sensor_pin);
  //right2_sensor_state = analogRead(right2_sensor_pin);
   
}
void back_off (char a,char b, int c)          //Move backward
{
  int TimeMillis = millis();
  while (TimeMillis - c < 500){
  Serial.println("---------------Backing off from bin");           
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Backward);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Backward);
  }
  
}
void swing_back_Robot(char a, char b, int c, char d){
  analogWrite (R_ME,a);
  analogWrite (L_ME,b);
  //int TimeMillis = millis();
  char from_side = d;
  
  while(TimeMillis - c < 1000) {
    Serial.println("---------------------Time to turn:  ");
    Serial.println(TimeMillis - c);
    
    if (from_side == "RIGHT"){        //Robot backing off from the right side 
    digitalWrite(R_M1,L_Backward);
    digitalWrite(L_M1,R_Forward);}
    else {                            //Robot backing off from the left side
    digitalWrite(R_M1,L_Forward);
    digitalWrite(L_M1,R_Backward);
    }
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  delay(100);               //Wait for a 1/10 of a sec
  if (right_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD && middle_sensor_state < THRESHOLD){
    keep_going = 1;
  }
  else
   advance_to_bin = 0;
}
void turn_L (char a,char b, int c)             //Turn Left
{
  analogWrite (R_ME,a);
  analogWrite (L_ME,b);
  //int TimeMillis = millis();
 
  while(TimeMillis - c < 1000) {                  //Time to turn towards bin
    Serial.println("---------------------Time to turn:  ");
    Serial.println(TimeMillis - c);
    digitalWrite(R_M1,L_Forward);
    digitalWrite(L_M1,R_Backward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  delay(100);               //Wait for a 1/10 of a sec
 // if (right_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD && middle_sensor_state < THRESHOLD){
    advance_to_bin = 1;
//  }
//  else
//   advance_to_bin = 0;
}
void turn_L_line (char a)             //Adjust Left
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Forward);
  analogWrite (L_ME,0);
  digitalWrite(L_M1,LOW);
}
void turn_R (char a,char b, int c)             //Turn Right
{  
  analogWrite (R_ME,a);
  analogWrite (L_ME,b);
  //int TimeMillis = millis();
 
  while(TimeMillis - c < 1000) {
    Serial.println("---------------------Time to turn:  ");
    Serial.print(TimeMillis - c);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  //delay(100);               //Wait for a 1/10 of a sec
  //if (right_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD && middle_sensor_state < THRESHOLD){
    advance_to_bin = 1;
//  }
//  else
//   advance_to_bin = 0;
//  
}
void turn_R_line (char b)             //Ajust Right
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

  // Middle 
  //left_sensor_pin,    INPUT);
  pinMode(middle_sensor_state, INPUT);        //added 2/20/20
  
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
int Right_bin = 1;
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
   //Serial.print(middle_sensor_state);
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
  if(right_sensor_state < THRESHOLD   && middle_sensor_state < THRESHOLD && left_sensor_state > THRESHOLD)          //turn_L_line(turn_speed);
   //remember to check
  {
    Serial.println("Adjust right L(1) M(1) R(0) ");
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
   if (right_sensor_state < THRESHOLD && middle_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD)      //turn_L_line(turn_speed1)
   {
    
    Serial.println("Adjust left L(0) M(0) R(1)");
    turn_L_line(turn_speed1);
   }

  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();

  // LEFT
  //     ||||   
  //      L|M|R
  //      ||||
  // L(1) M(1) R(0)
  // <-
  if (right_sensor_state > THRESHOLD && middle_sensor_state < THRESHOLD && left_sensor_state < THRESHOLD)      //turn_R_line(turn_speed);
  {
    Serial.println("Adjust right L(1) M(1) R(0) ");
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
     if (right_sensor_state > THRESHOLD   && middle_sensor_state > THRESHOLD && left_sensor_state < THRESHOLD)    //turn_R_line(turn_speed1);
     {
    Serial.println("Adjust right L(1) M(0) R(0) ");
    turn_R_line(turn_speed1);
    }
    
  //-----------------------
  // IR Sensors: READ
  //-----------------------
   read_ir();
  
  // STOP
   if (right_sensor_state < THRESHOLD && middle_sensor_state < THRESHOLD && left_sensor_state < THRESHOLD && advance_to_bin == 0){
     back_off_from_bin = 0;                             //The robot just drove back from the bin. It no longer needs to drive backwards.
     if (white_lines == n && swing_back != 1)           //Robot driving forward from the starting Square
     {
        if (Right_Bin == 1)
          {
            Serial.println("--------------------------Turning towards Right Bin");
            int turnMillis = millis();
//            Serial.println("turnMillis = ");
//            Serial.print(turnMillis);
            turn_R(motor_speed_L, motor_speed_R, turnMillis);         //Robot turns towards the Right side bins
            Right_Bin = 0;                                          //Can be an array sequence, so Robot knows which side to turn in next
            from_side = "RIGHT";                                  //Notes which side the robot is driving towards 
            advance_to_bin = 1;                                 //Allows the robot to drive from the line to the Bin
            swing_back = 1;
            }
         else if (Left_Bin == 1)
          {
            Serial.println("--------------------------Turning towards Left Bin");
            int turnMillis = millis();
//            Serial.println("turnMillis = ");
//            Serial.print(turnMillis);
            turn_R(motor_speed_L, motor_speed_R, turnMillis);        //Robot turns towards the Left side bins
            Left_Bin = 0;                                           //Can be an array sequence, so Robot knows which side to turn in next
            from_side = "LEFT";                                     //Notes which side the robot is driving towards
            advance_to_bin = 1;                                 //Allows the robot to drive from the line to the Bin
            swing_back = 1;
            }
        else 
        Serial.println("stop");
        stopp();
        }
     else if (swing_back == 1 && back_off_from_bin == 1)                      //Robot driving backward from the left or right bins
     {
      Serial.println("--------------------------Coming from Bin, Swinging back on the Line");
            int turnMillis = millis();
//            Serial.println("turnMillis = ");
//            Serial.print(turnMillis);
            swing_back_Robot(motor_speed_L, motor_speed_R, turnMillis, from_side);                //Takes into account, which side the robot needs to swing back from
            swing_back = 0;                                                               //Robot should be perfectly on the line again. 
            back_off_from_bin == 0;
     }
                                                                
     else                                     //Normally driving forward
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
        Serial.print("=====================================================Swing Back value");
        Serial.println(swing_back);
        

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
   
  //------------------------
  // GO TO BIN
  //------------------------
   if (right_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD && middle_sensor_state < THRESHOLD && advance_to_bin == 1)
    {
       
       Serial.print("-------------------------------------Going towards the Bin ");
       Serial.print("-------------------------------------Bin Number: ");
       Serial.println(white_lines);
       
       int TimeMillis = millis();
        go_to_bin(motor_speed_R, motor_speed_L, TimeMillis);
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
       advance_to_bin == 0;
       back_off_from_bin = 1;
       //--------------------------------
       //call pick up block functions
       //---------------------------------
       delay(100);
       //go_back = 1;
    }
  // BACK OFF FROM BIN
if (right_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD && middle_sensor_state > THRESHOLD && back_off_from_bin == 1)
                                                //Will back off from  the bin when called and when on Black 
  {
    
    Serial.println("-----------------------going backward from the Bin");
    int TimeMillis = millis();
    back_off(motor_speed_R,motor_speed_L, TimeMillis);

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
      back_off_from_bin = 0;                //Will no longer back off from the bin when on black
      swing_back = 1;       
  }
  // FORWARD
  if(right_sensor_state > THRESHOLD && left_sensor_state > THRESHOLD && middle_sensor_state < THRESHOLD && advance_to_bin != 1)
  
  {
    

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
        cnt++;
       
        counter_R = 0;
        counter_L = 0;
        
  }
   previousMillis = currentMillis;
  //delay(500);
}
