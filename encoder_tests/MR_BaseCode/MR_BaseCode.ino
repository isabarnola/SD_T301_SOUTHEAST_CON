
      #include "mrobot.h"

#define ENCODEROUTPUT 4741.44
#define MIN 80
#define motor_speed 100 // MAX = 255
int motor_speed_R  = 100;//95;//85;// MAX = 255
int motor_speed_L =100;//105;//95; // MAX = 255
#define turn_speed  75
#define BLACK 150
#define L_Forward HIGH
#define R_Forward LOW
#define L_Backward LOW
#define R_Backward HIGH
//-----------------------
// Motor : Right
//-----------------------
int R_ME  = 4;   //Enable Pin of the Right Motor (must be PWM)
int R_M1 = 50;    //Control Pin

//-----------------------
// Motor : Left
//-----------------------
int L_ME  = 6;   //Enable Pin of the Left Motor (must be PWM)
int L_M1 = 51;

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

      void setup() {
              
               setup_timer(); /*This will setup an Interrupt Service Routine      to be executed at 10ms*/
               encoder_init();
                //motor_init();
                 // Right
                  pinMode(R_M1, OUTPUT);

                 // Left
                  pinMode(L_M1, OUTPUT);
               Serial.begin(9600);
       }

      void loop() {
       advance(100,100); 
      Serial.print(encoder0_val);
      Serial.print("\t");
      Serial.print(encoder0_val);
      Serial.print("\t");
      Serial.print(cur_wvel[0]);
        Serial.print("\t");
      Serial.println(cur_wvel[1]); 
      delay(100);
        
      }

        void control(void){
        get_current_status();
       
        
          /******************************/ 
       // des_wvel[0] = 1.0;  // set motor 0 to 1 rad/sec  
       // des_wvel[1] = 1.0;  // set motor 1 to 1 rad/sec
           /********************************/ 
       // low_level_control(); 
        }

        
        /**This function will be executed every 10ms **/
        ISR(TIMER1_COMPA_vect) /* timer compare interrupt service routine*/
        {
  
          control();
        }
