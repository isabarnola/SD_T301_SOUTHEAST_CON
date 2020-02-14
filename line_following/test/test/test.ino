
#include "encoder.h"

//-----------------------
// Motor : Right
//-----------------------
int R_M_PWM  = 4;   //Enable Pin of the Right Motor (must be PWM)
int R_M_E = 50;    //Control Pin

//-----------------------
// Motor : Left
//-----------------------
int L_M_PWM  = 6;   //Enable Pin of the Left Motor (must be PWM)
int L_M_E = 51;

void advance(char a,char b)          //Move forward
{
  
  digitalWrite(L_M_E,LOW);
   //delay(1);
  digitalWrite(R_M_E,LOW);
  // delay(1);
  analogWrite (L_M_PWM,a);  //PWM Speed Control
    //delay(1);
  analogWrite (R_M_PWM,b);
  delay(1);
  
}

void setup() {
  // put your setup code here, to run once:
  //-----------------------
  // Motors
  //-----------------------

  // Right
  pinMode(R_M_E, OUTPUT);

  // Left
  pinMode(L_M_E, OUTPUT);

   encoder_init();

   Serial.begin(9600); 
     Serial.println("M0\t\t\tM1\t\t\t" );
}

int compute0(void)
{
  }

void loop() {

   advance(100,115);
   get_current_status();
    Serial.print("encoder0 " );
    Serial.print(encoder0_val);
    Serial.print("\tencoder1 " );
    Serial.print(encoder1_val);
    
   Serial.print("\tcur_wpos[0] " );
    Serial.print(cur_wpos[0]);
    Serial.print("\tcur_wpos[1] " );
    Serial.print(cur_wpos[1]);
   
    Serial.print("\tcur_wvel[0] " );
    Serial.print(cur_wvel[0]);
    Serial.print("\tcur_wvel[1] " );
    Serial.println(cur_wvel[1]);

    
    
  // put your main code here, to run repeatedly:

}
