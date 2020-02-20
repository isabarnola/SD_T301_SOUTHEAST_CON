
            /*function prototypes */
            
            void pwm_init(void);             /*initialize pwm*/
            void direction_init(void);       /*initialize direction pins*/
            
            void pwm_set_duty(int channel, int duty);
            void set_direction(int channel, int dir);
            
            void motor_init(void);          /* call pwm_init and direction_init */ 
            void set_motor_duty(int channel, int duty); 
            
            void setup_timer(void); 
            void get_current_status(void);
            
            void ADC_init(void);           /*initiliaze analog to digital for IR sensor*/
            int ADC_read(uint8_t channel); /* read analog channel 
                                          /* channel = 0 for A0 - right sensor 
                                             channel = 1 for A1 - front sensor 
                                             channel = 2 for A2 - left sensor*/
            

      

            #define PI 3.1416

            /****** Motor Function ******/
 
            
            void motor_init(void)
            {
              pwm_init();
              direction_init();
            }
            
            void set_motor_speed(int channel, int duty)
            {
                pwm_set_duty(channel,duty);
            }
            
            void direction_init(void){
                pinMode(50,OUTPUT); //motor right enable
                pinMode(51,OUTPUT); //motor left enable
                
            } /*end of direction_init*/
            
            
            void pwm_init(void){//pwm set up
                pinMode(4, OUTPUT); //motor 0
                pinMode(6, OUTPUT); //motor 1
                TCCR5A = _BV(COM5A1) | _BV(COM5B1) | _BV(WGM52) | _BV(WGM50);
                TCCR5B = _BV(CS51) | _BV(CS50);  //set prescaler to 128
                OCR5A  =  0;
                OCR5B  =  0;
            } /*end of pwm_init*/
            
            
            
            void pwm_set_duty(int channel, int duty)
            { 
               
                if (duty > 255){ 
                  duty = 255;
                  }
                  else if(duty < -255){
                  duty = -255;
                  }
            
                  
                  if (duty>0) {
                  set_direction(channel,1);      // forward 
                  }
                  else if(duty<0) {
                    set_direction(channel, 0);   //reverse
                    duty = -duty;
                    }
                    
                 if (channel==0)  //motor 0 (left motor)
                    OCR5B = duty; //pin45
                    
                 else if (channel ==1)//motor 1(right motor)
                    OCR5A = duty; //pin46
            }
            /*end of pwn_set_duty*/
            
            
            void set_direction(int channel, int dir)//set motor direction
            {
                if (channel == 1)//motor 1(right)
                 {
                  if(dir == 0) //reverse 
                   {
                    digitalWrite(50,HIGH);  //I3
                    digitalWrite(51,HIGH); //I4
                    }
                   else if(dir == 1) //forward
                   {
                    digitalWrite(50,LOW); //I3
                    digitalWrite(51,LOW);  //I4
                    }
                    
                  } /*end if channel ==1 */
                  
                 else if(channel == 0) //motor 1(left)
                 {
                  if(dir == 0) //reverse
                   {
                    digitalWrite(50,LOW);  //I1
                    digitalWrite(51,HIGH); //I2
                    }
                   else if(dir == 1) //forward
                   {
                    digitalWrite(50,HIGH); //I1
                    digitalWrite(51,LOW);  //I2
                   }
                 }  /*end else if */
            } /*end set_direction*/
            
            
            /****************************  Encoder Functions  *********************************/ 




          void encoder_init(void);
          void encoder0CHB(void);
          void encoder0CHA(void);
          void encoder1CHB(void);
          void encoder1CHA(void);

          const byte interruptPIN2 = 2;
          const byte interruptPIN3 = 3;
          const byte interruptPIN0 = 18;
          const byte interruptPIN1 = 19;

          byte state0 = 0;
          byte state1 = 0;
          
          long int encoder0_val=0;
          long int encoder1_val=0;

          void encoder_init(void){
            
                  pinMode(interruptPIN2, INPUT_PULLUP);
                  pinMode(interruptPIN3, INPUT_PULLUP);
                  attachInterrupt(digitalPinToInterrupt(interruptPIN2), encoder0CHA, CHANGE);//set GPIO pin for encoder
                  attachInterrupt(digitalPinToInterrupt(interruptPIN3), encoder0CHB, CHANGE);//set GPIO pin for encoder


                  
                  pinMode(interruptPIN0, INPUT_PULLUP);
                  pinMode(interruptPIN1, INPUT_PULLUP);
                  attachInterrupt(digitalPinToInterrupt(interruptPIN0), encoder1CHA, CHANGE);//set  GPIO pin for encoder
                  attachInterrupt(digitalPinToInterrupt(interruptPIN1), encoder1CHB, CHANGE);//set  GPIO pin for encoder
          }


        /******* Encoder 0 for Motor 0 ************************/
          
        void encoder0CHA(void) //function to read and determine state of encoder 0-A
          {
              if ((state0 == 0) && (digitalRead(interruptPIN2) ==1))
                    {
                          state0 =1;
                          encoder0_val +=1;
                    }
              else if ((state0==1) && (digitalRead(interruptPIN2) ==0))
                    {
                          state0 =0;
                          encoder0_val -=1;
                    }
               else if ((state0==2) && (digitalRead(interruptPIN2) ==1))
                    {
                          state0 =3;
                          encoder0_val -=1;
                    }
              else if ((state0==3) && (digitalRead(interruptPIN2) ==0))
                   {
                          state0 =2;
                          encoder0_val +=1;
                    }
            }

            
        void encoder0CHB(void) //function to read and determien state of encoder 0-B
              {
                if ((state0 == 0) && (digitalRead(interruptPIN3) ==1))
                   {
                    state0 =2;
                    encoder0_val -=1;
                   }
                else if ((state0==1) && (digitalRead(interruptPIN3) ==1))
                {
                  state0 =3;
                  encoder0_val +=1;
                  }
                  else if ((state0==2) && (digitalRead(interruptPIN3) ==0))
                  {
                  state0 =0;
                  encoder0_val +=1;
                  }
                  else if ((state0==3) && (digitalRead(interruptPIN3) ==0))
                  {
                    state0 =1;
                    encoder0_val -=1;
                  }
              }
    

   /***************** Encoder 1 for Motor 1 ********************/

       void encoder1CHA(void) 
        {
           if ((state1 == 0) && (digitalRead(interruptPIN0) == 1))
             {
              state1 =1;
              encoder1_val +=1;
             }
            else if ((state1 == 1) && (digitalRead(interruptPIN0) == 0))
           {
            state1 =    0;
            encoder1_val -=1;
            }
            else if ((state1 == 2) && (digitalRead(interruptPIN0) == 1))
            {
            state1 =3;
            encoder1_val -=1;
            }
            else if ((state1 == 3) && (digitalRead(interruptPIN0) == 0))
            {
              state1 =2;
              encoder1_val +=1;
            }
        }
        
        void encoder1CHB(void) 
        {
          if ((state1 == 0) && (digitalRead(interruptPIN1) ==1))
        {
                state1 = 2;
                encoder1_val -=1;
             }
          else if ((state1 == 1) && (digitalRead(interruptPIN1) ==1))
          {
              state1 =3;
              encoder1_val +=1;
            }
            else if ((state1 == 2) && (digitalRead(interruptPIN1) ==0))
            {
              state1 =0;
              encoder1_val +=1;
            }
            else if ((state1 == 3) && (digitalRead(interruptPIN1) ==0))
            {
              state1 =1;
              encoder1_val -=1;
            }
        }  



   /***********  Timer Function for creating deterministic clock *******************/

   
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


void low_level_control(void)
{
  float voltage[2] ={0.,0.};
  float Kp = 50.;
  float Kd = 5;
  int   duty = 0;
  float Vcc  = 7.2; /* Battery Voltage*/


  /*PD Control*/

  
  voltage[0] = Kp*(des_wpos[0]-cur_wpos[0]) + Kd*(des_wvel[0] - cur_wvel[0]);
  duty      = voltage[0]/Vcc * 255;
  
  set_motor_speed(0,duty);

  

  voltage[1] = Kp*(des_wpos[1]-cur_wpos[1]) + Kd*(des_wvel[1] - cur_wvel[1]);
  duty       = voltage[1]/Vcc * 255;
  
  set_motor_speed(1,duty);

  
}




            
            

