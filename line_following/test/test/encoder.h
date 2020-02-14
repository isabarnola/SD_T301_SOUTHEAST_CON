
            /****************************  Encoder Functions  *********************************/ 




          void encoder_init(void);
          void encoder0CHB(void);
          void encoder0CHA(void);
          void encoder1CHB(void);
          void encoder1CHA(void);

          const byte interruptPIN2 = 2;
          const byte interruptPIN3 = 35;
          const byte interruptPIN0 = 3;
          const byte interruptPIN1 = 31;

          byte state0 = 0;
          byte state1 = 0;
          
          long int encoder0_val=0;
          long int old_0       =0;
          long int vel_0       = 0;
          long int encoder1_val=0;
          long int vel_1       = 0;
          long int old_1       = 0;
          long int new_time0 = 0;
          long int old_time0 = 0;
          long int new_time1 = 0;
          long int old_time1 = 0;
          long int time0 = 0;
          long int time1 = 0;



          void encoder_init(void){
            
                  pinMode(interruptPIN2, INPUT_PULLUP);
                  //pinMode(interruptPIN3, INPUT_PULLUP);
                  attachInterrupt(digitalPinToInterrupt(interruptPIN2), encoder0CHA, CHANGE);//set GPIO pin for encoder
                  //attachInterrupt(digitalPinToInterrupt(interruptPIN3), encoder0CHB, CHANGE);//set GPIO pin for encoder


                  
                  pinMode(interruptPIN0, INPUT_PULLUP);
                  //pinMode(interruptPIN1, INPUT_PULLUP);
                  attachInterrupt(digitalPinToInterrupt(interruptPIN0), encoder1CHA, CHANGE);//set  GPIO pin for encoder
                  //attachInterrupt(digitalPinToInterrupt(interruptPIN1), encoder1CHB, CHANGE);//set  GPIO pin for encoder
          }


        /******* Encoder 0 for Motor 0 ************************/
          
        void encoder0CHA(void) //function to read and determine state of encoder 0-A
          {
              if ((digitalRead(interruptPIN2) ==1))
                encoder0_val++;
            
            
                
            }

            


   /***************** Encoder 1 for Motor 1 ********************/

       void encoder1CHA(void) 
        {
           if ((digitalRead(interruptPIN0) == 1))
             {
              encoder1_val +=1;
             }
      
          
        }




void setup_timer(void)
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 625;   //compare match register 16MHz/256/2Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();
}



/*** This function determines the wheel velocities of the mobile robot **/


float cur_wpos[2]  = {0.,0.};  // current wheel position 0 - left  1 - right
float cur_wvel[2]  = {0.,0.};  // current wheel velocity 0  left   1 right
float des_wpos[2]  = {0.,0.};  // desired wheel position
float des_wvel[2]  = {0.,0.};  // desired wheel velocity
float prev_wpos[2] = {0.,0.};  // previous wheel position
float prev_des_wpos[2] = {0.,0.};  // previous desired wheel position




#define END_CPR     100
#define Gear_Ratio  99
#define T           0.01  // 10 msec

void get_current_status(void)
{
  int i;
  
  /*determine current wheel position in rad*/
  
  
  cur_wpos[0] = encoder0_val*2*PI/(4.0*END_CPR*Gear_Ratio);
  cur_wpos[1] = encoder1_val*2*PI/(4.0*END_CPR*Gear_Ratio);
  
  
  for(i=0;i<2;i++)
  {
    cur_wvel[i]  = (cur_wpos[i] - prev_wpos[i])/T;
    prev_wpos[i] = cur_wpos[i];
  }
  
  /*determine des_wheel position*/
  for (i=0;i<2;i++)
  {
    des_wpos[i] = prev_des_wpos[i] + des_wvel[i]*T;
    prev_des_wpos[i] = des_wpos[i];
  }
  
}

