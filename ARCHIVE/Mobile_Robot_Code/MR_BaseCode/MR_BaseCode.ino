
      #include "mrobot.h"
     

      void setup() {
              
               setup_timer(); /*This will setup an Interrupt Service Routine      to be executed at 10ms*/
               encoder_init();
                //motor_init();
               
               Serial.begin(9600);
       }

      void loop() {

      Serial.println(encoder0_val);
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
