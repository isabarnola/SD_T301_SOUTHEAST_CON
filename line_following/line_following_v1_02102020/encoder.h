
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
