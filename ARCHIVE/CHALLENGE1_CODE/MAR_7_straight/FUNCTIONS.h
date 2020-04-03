// ================================================================
// Function
// ================================================================

void print_state()
{
  if (state == STOP)
  {
    Serial.println("STATE = STOP ###################");
  }
  else if (state == LINE_FOLLOWING)
  {
    Serial.println("STATE = LINE_FOLLOWING ###################");
  }
  else if (state == TURN_LEFT_BIN)
  {
    Serial.println("STATE = TURN_LEFT_BIN ####################");
  }
  else if (state == TURN_RIGHT_BIN)
  {
    Serial.println("STATE = TURN_RIGHT_BIN ###################");
  }
  else if (state == TURN_LEFT_LINE)
  {
    Serial.println("STATE = TURN_LEFT_LINE ###################");
  }
  else if (state == TURN_RIGHT_LINE)
  {
    Serial.println("STATE = TURN_RIGHT_LINE ##################");
  }
  else if(state == GO_TO_BIN)
  {
    Serial.println("STATE = GO_TO_BIN ########################");
  }
  else if(state == BACK_TO_LINE)
  {
    Serial.println("STATE = BACK_TO_LINE #####################");
  }
  else if (state == STACK)
  {
    Serial.println("STATE = STACK ############################");
  }
  else if (state == TURN_180)
  {
   Serial.println("STATE = TURN_180 ##########################");
  }
}

int set_state()
{
  if (itr_s < num_states_to_do -1)
  {
    itr_s = itr_s +1;
    return states_to_do[itr_s];
  }
    
  return 0 ;
}


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
void read_ir()
{
  left_sensor_state         =  digitalRead(left_sensor_pin);
  right_sensor_state        =  digitalRead(right_sensor_pin);
  middle_sensor_state       =  digitalRead(middle_sensor_pin);
  left_arm_sensor_state     =  digitalRead(left_arm_sensor_pin);
  right_arm_sensor_state    =  digitalRead(right_arm_sensor_pin);
  front_right_sensor_state  =  digitalRead(front_right_pin);
  front_left_sensor_state   =  digitalRead(front_left_pin);
  low_right_sensor_state    =  (digitalRead(low_right_pin));
  low_left_sensor_state     =  (digitalRead(low_left_pin));
}
void back_off (char a,char b)          //Move backward
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Backward);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Backward);
}
void turn_L (char a,char b)             //Turn Left
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Forward);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Backward);
}
void turn_L_line (char a)             //Turn Left
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Forward);
  analogWrite (L_ME,0);
  digitalWrite(L_M1,LOW);
}
void turn_R (char a,char b)             //Turn Right
{
  analogWrite (R_ME,a);
  digitalWrite(R_M1,R_Backward);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Forward);
}
void turn_R_line (char b)             //Turn Right
{
  analogWrite (R_ME,0);
  digitalWrite(R_M1,LOW);
  analogWrite (L_ME,b);
  digitalWrite(L_M1,L_Forward);
}
void go_straight()
{
  int temp1 = abs(cur_wvel[1] -  cur_wvel[0]);
  
  if (temp1 >= 0.05 && temp1 <= 0.5 )
  {
    //speed_L =speed_L;
    speed_R =speed_R;
    }
   else if (cur_wvel[1] > cur_wvel[0] )
      speed_R--;
      //speed_L--;
    else if (cur_wvel[1] < cur_wvel[0])
      speed_R++;
      //speed_L++;
}

void go_straight2()
{
  int temp1 = abs(cur_wvel[1] -  cur_wvel[0]);
  
  if (temp1 >= 0.05 && temp1 <= 0.5 )
  {
    slow_L =slow_L;
    }
   else if (cur_wvel[1] > cur_wvel[0] )
      slow_L--;
    else if (cur_wvel[1] < cur_wvel[0])
      slow_L++;
}

void fix_turn_r_90_line(int right, int left)
{
  read_ir();
  while(left_sensor_state != BLACK && middle_sensor_state != BLACK && left_sensor_state != BLACK)
  {
    analogWrite (R_ME,right);
    analogWrite (L_ME,left);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    read_ir();
  }
}
void fix_turn_l_90_line(int right, int left)
{
  read_ir();
  while(left_sensor_state != BLACK && middle_sensor_state != BLACK && left_sensor_state != BLACK)
  {
    
    analogWrite (R_ME,right);
    analogWrite (L_ME,left);
    digitalWrite(R_M1,R_Backward);
    digitalWrite(L_M1,L_Forward);
    read_ir();
  }
}


void turn_90_r(int right, int left, int time, int time_to_turn)
{
  analogWrite (R_ME,right);
  analogWrite (L_ME,left);
  int TimeMillis = 0;
 
  while(TimeMillis - time < time_to_turn)
   {                  //Time to turn towards bin
    Serial.println("---------------------Time to turn:  ");
    Serial.println(TimeMillis - time);
    digitalWrite(R_M1,R_Forward);
    digitalWrite(L_M1,L_Backward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  //fix_turn_r_90_line(right,left);
}

void turn_90_l(int right, int left, int time, int time_to_turn)
{
  analogWrite (R_ME,right);
  analogWrite (L_ME,left);
  int TimeMillis = 0;
 
  while(TimeMillis - time < time_to_turn)
   {                  //Time to turn towards bin
    Serial.println("turn 90 l---------------------Time to turn:  ");
    Serial.println(TimeMillis - time);
    digitalWrite(R_M1,R_Backward);
    digitalWrite(L_M1,L_Forward);
    TimeMillis = millis();
  }
  analogWrite (R_ME,0);
  analogWrite (L_ME, 0);
  
  //fix_turn_l_90_line(right,left);
}

void back_before_turn(int timee)
{
  Serial.println("FUNCTION Back before turn");
  long int TimeMillis = 0;
    
    long int time = millis();
    while(TimeMillis - time < timee) //700)
    {                  //Time to turn towards bin
      Serial.println(" back_before_turn() ---------------------backwardsn:  ");
      Serial.println(TimeMillis - time);
      
      back_off(speed_R,speed_L);
      get_current_status();
      go_straight();
      
      TimeMillis = millis();
    }
}

void advance_before_tll(int timee)
{
  Serial.println("FUNCTION  advance_before_tll");
  long int TimeMillis = 0;
    
    long int time = millis();
    while(TimeMillis - time < timee) //700)
    {                  //Time to turn towards bin
      Serial.println("  advance_before_tll:  ");
      Serial.println(TimeMillis - time);
      get_current_status();
      go_straight();
      advance(speed_R,speed_L);
      TimeMillis = millis();
    }
  
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
    int forward = 0;
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
          
            
         if (invert == 1)
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
    Serial.println("==========aligned_bin_right=======");
     read_ir();
    
    int exit = 0;
    int TimeMillis = 0;
    int invert = 0;
    int forward = 0;
    while (exit == 0)
    {
         Serial.print("forward ");
            Serial.println(forward);
        Serial.print(front_left_sensor_state);
        Serial.print("\t");
        Serial.print(front_right_sensor_state);
        Serial.print("\t");
        Serial.print("front_left_sensor_state ");
        Serial.println(front_left_sensor_state);

        //Serial.println(TimeMillis);
        
        if (TimeMillis - time1 < correct_time)
          invert =1;

        Serial.print("Invert ");
        Serial.println(invert);
        
        read_ir();

        //L(B) M(?) R(W)
        if (front_left_sensor_state == BLACK && front_right_sensor_state == WHITE )
        {
          //Serial.println("front_left_sensor_state == BLACK && front_right_sensor_state == WHITE");
          //back_off(0, turn_speed);
          if (invert == 0)
          {
             //advance(turn_speed+7, 0);
             advance(0, turn_speed+15);
            Serial.println("advance(0, turn_speed+7)");
          }
          //advance(0, turn_speed -7);
          else
          {
            //back_off(turn_speed+7, 0); // worked
            back_off(0, turn_speed+10);
            Serial.println(" back_off(0, turn_speed+7);");
          }
          
          
        }
        
        else if (front_left_sensor_state == WHITE && front_right_sensor_state == BLACK )
        {
          //Serial.print(" front_left_sensor_state == WHITE && front_right_sensor_state == BLACK left correction");
          //back_off(turn_speed, 0);
          if (invert == 0)
          {
            //advance(0, turn_speed+7); // worked = turn_l_line
            advance(turn_speed+10, 0);
             Serial.println(" advance(turn_speed+7, 0)");
          }
          //advance  (0,turn_speed -7);
          else
          {
            //back_off(0,turn_speed+7); // worked
            back_off(turn_speed+10, 0) ;
            Serial.println("back_off(turn_speed+7, 0) ;");
          }
          
        }
        else if(front_left_sensor_state == BLACK && front_right_sensor_state == BLACK)
        {
          
            
            
         if (invert == 0 )
         {
            back_off (turn_speed, turn_speed);
            Serial.println("Invert == 1back_off (turn_speed, turn_speed);");
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



void line_following(int lines, int dir)
{
  while(white_lines != lines)
  {
    read_ir();
      // LEFT
      //      | | |  
      //       L M  R
      //      | | |
      // <- L
      // L(W) M(W) R(B)
      if(left_sensor_state == WHITE      && middle_sensor_state == WHITE    && right_sensor_state == BLACK )
      {
        Serial.println("LINE FOLLOWING  turning left L(W) M(W) R(B) ");
         advance(speed_R, speed_L+ plus_factor);
         //advance(speed_R + plus_factor , speed_L);
         Serial.println("advance(speed_R, speed_L+ plus_factor);");
         
      }
      else
      {  // opening else - right eft - left - stop - forward 
        read_ir();
        // RIGHT
        //      | | |  
        //       R
        //     M  | |
        //   L  | | |  
        //-> R
        // L(B) M(B) R(W)
      if (left_sensor_state == BLACK       && middle_sensor_state == BLACK    &&  right_sensor_state == WHITE )
      {
        Serial.println("LINE FOLLOWING  going right L(B) M(B) R(W)");
         advance(speed_R + plus_factor , speed_L);
         //advance(speed_R , speed_L + plus_factor);
         Serial.println("advance(speed_R + plus_factor, speed_L);");
      }
      else
      {// opening else - left - left - stop - forward
        read_ir();
        // RIGHT
        //     ||||   
        //   L  M R
        //     ||||
        // L(B) M(W) R(W)
        // ->R
          if(left_sensor_state == BLACK     && middle_sensor_state == WHITE    && right_sensor_state == WHITE )
        {// opening if - go right
          Serial.println("LINE FOLLOWING going left L(B) M(W) R(W)");
          //advance(speed_R +8, speed_L );
          
         // advance(speed_R , speed_L + plus_factor );
            advance(speed_R + plus_factor, speed_L);

         
          //advance(speed_R  + plus_factor , speed_L);
          Serial.println(" advance(speed_R , speed_L + plus_factor);");
           
          
        }// close if - go left
        else
        { // opening else - left - stop - forward
          read_ir();
          // LEFT
          // L(W) M(B) R(B)
          //      | | |  
          //        L  
          //      | |  M 
          //      | |    R 
          // <-
          if (left_sensor_state == WHITE && middle_sensor_state == BLACK && right_sensor_state == BLACK  )
          { // opening if - going left
            Serial.println("LINE FOLLOWING turning right L(W) M(B) R(B) ");
             advance(speed_R , speed_L + plus_factor);
             //advance(speed_R + plus_factor, speed_L );
             Serial.println("advance(speed_R , speed_L + plus_factor);");
              
          } // closing if - going left
          else
          { // opening else - stop & forward
            read_ir();
            // STOP
            if(left_sensor_state == WHITE && middle_sensor_state == WHITE && right_sensor_state == WHITE)
            { // opening if - stop
              if (white_lines == lines)
              { // opening if - stop
                  Serial.println("LINE FOLLOWING stop          =       =       =     =         =       =       =       =       =     =");
                  stopp();
                  
                  //state = TURN_TO_BIN;
              } // closing if - stop
              else
              { // opening else - pass white line
                while(left_sensor_state == WHITE && middle_sensor_state == WHITE && right_sensor_state == WHITE) 
                { // opening while loop - pass white line
                      read_ir();
                    Serial.print("|||||||||||||||| PASSING WHITE||||||||||||||||||  ");
                    Serial.println(white_lines);
                    get_current_status();     // get velocity
                 
                    advance(speed_R,speed_L); // move forward
                    go_straight();            // correct velocity
                     e_turn =0;
                } // closing while loop - pass white line
                white_lines++;
               // state++;
              } // close else - pass white line
            } // closing if - stop
            else
            { //opening else - froward
              read_ir();
              // FORWARD
              if(left_sensor_state == BLACK && middle_sensor_state == WHITE && right_sensor_state == BLACK)  // ASK ISA 
              { // opening if - forward
                Serial.println("LINE FOLLOWING: going forward");
                get_current_status();             // get velocity
               // Serial.println(speed_L);
               // Serial.print("\t");
               // Serial.print(speed_R);    
                advance(speed_R,speed_L);         // advance
                go_straight();                    // correct velocity
                    e_turn =0;
              } // closing if - forward
            } //closing else - froward 
          } // closing else - stop & forward
        } // closing else - left - stop - forward
      }// closing else - left - left - stop - forward
    }// closing else - right eft - left - stop - forward 
  }

  if (dir == plus_dir)
    pos = pos + white_lines;
  else if (dir == minus_dir)
    pos = pos - white_lines;

  white_lines = 0;
}


void turn_left_bin(int turntime)
{
    turn_90_l(speed_R,speed_R, millis(), turntime);   
    delay(1000);
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
  analogWrite (R_ME,speed_R+10);
  analogWrite (L_ME,speed_L);
  int TimeMillis = 0;
  int correct = 0;
 int time1 = millis();
  while(TimeMillis - time1 < turntime)
   {                  //Time to turn towards bin
    Serial.println("turn_left_line ---------------------Time to turn:  ");
    Serial.println(TimeMillis - time1);
    digitalWrite(R_M1,R_Backward);
    digitalWrite(L_M1,L_Forward);
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


void back_to_line()
{   
  Serial.println("BACK TO LINE");

   int r = speed_R;
   int l = speed_L;
   speed_R = speed_R -20;
   speed_L = speed_L + 6;
    read_ir();
     while(left_sensor_state == BLACK || right_sensor_state == BLACK)
    { //Time to turn backwards bin
      //get_current_status();
      back_off(speed_R,speed_L);
      //go_straight();
      read_ir();

      Serial.print("SPEED_back_to_line ");
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
   Serial.println("BACK TO LINE FRONT");

   int r = speed_R;
   int l = speed_L;
   speed_R = speed_R -15;
   speed_L = speed_L;
   int l1_r2= 0;
   int r1_l2 = 0;
    read_ir();
     while((front_left_sensor_state == BLACK || front_right_sensor_state == BLACK) && (r1_l2 != 2 || l1_r2 != 2 ))
    { //Time to turn backwards bin

      if (front_left_sensor_state == WHITE )
      {
          Serial.print("l1_r2 ");
            Serial.println(l1_r2);
          l1_r2 = 1;
      }
      if(front_right_sensor_state == WHITE )
      {
        Serial.print("r1_l2 ");
            Serial.println(r1_l2);
          r1_l2 = 1;
      }
           
            

       if (l1_r2 == 1 && front_right_sensor_state == WHITE)
       {
         l1_r2 = 2;
         Serial.print("l1_r2 ");
            Serial.println(l1_r2);
        }

           if (r1_l2 == 1 && front_left_sensor_state == WHITE)
       {
         Serial.print("r1_l2 ");
            Serial.println(r1_l2);
          r1_l2 = 2;
        }
      
     // get_current_status();
      back_off(speed_R,speed_L);
      //go_straight();
      read_ir();

      Serial.print("SPEED_back_to_line_front");
      Serial.print(speed_L);
      Serial.print("\t");
      Serial.println(speed_R);
    }
    //get_current_status();
    back_off(speed_R,speed_L);
    //go_straight();
    speed_R = r;
    speed_L= l;
  
  }



void turn_right_bin(int turntime)
{
    Serial.print("turn_right_bin(");
    turn_90_r(speed_R,speed_R, millis(), turntime);  
    delay(100);
    back_to_line_front();
    delay(300);
    aligned_bin_right(millis(),1000); 
}


void go_to_bin()
{
  Serial.println("GO TO BIN FUNCTION");
  int exit = 0;
    int tempr = speed_R;
    int templ = speed_L;
    speed_R =  speed_R - 10;
    speed_L =  speed_L - 10;
  

  
    int TimeMillis = 0;
    int correct = 0;
   int time1 = millis();
    while(TimeMillis - time1 < 1500)
     {                  //Time to turn towards bin
      Serial.println("Arbitrary time going forward to avoid line detection:  ");
      Serial.println(TimeMillis - time1);
      advance(speed_R, speed_L);
      TimeMillis = millis();
      read_ir();
    }
  
    
    while (exit == 0)
    {
        Serial.print("front_left_sensor_state ");
        Serial.println(front_left_sensor_state);
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
