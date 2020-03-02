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
void read_ir()
{
  left_sensor_state         =  digitalRead(left_sensor_pin);
  right_sensor_state        =  digitalRead(right_sensor_pin);
  middle_sensor_state       =  digitalRead(middle_sensor_pin);
  left_arm_sensor_state     =  digitalRead(left_arm_sensor_pin);
  right_arm_sensor_state    =  digitalRead(right_arm_sensor_pin);
  front_right_sensor_state  =  digitalRead(front_right_pin);
  front_left_sensor_state   =  digitalRead(front_left_pin);
  low_right_sensor_state    =  digitalRead(low_right_pin);
  low_left_sensor_state     =  digitalRead(low_left_pin);
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
    speed_L =speed_L;
    }
   else if (cur_wvel[1] > cur_wvel[0] )
      speed_L--;
    else if (cur_wvel[1] < cur_wvel[0])
      speed_L++;
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

void back_before_turn()
{
  long int TimeMillis = 0;
    
    long int time = millis();
    while(TimeMillis - time < 700)
    {                  //Time to turn towards bin
      Serial.println(" back_before_turn() ---------------------backwardsn:  ");
      Serial.println(TimeMillis - time);
      back_off(speed_L,speed_R);
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
        // advance(speed_R, speed_L+ plus_factor);
         advance(speed_R + plus_factor , speed_L);
      }
      else
      {  // opening else - right eft - left - stop - forward 
        read_ir();
        //      | | |  
        //       R
        //     M  | |
        //   L  | | |  
        //-> R
        // L(B) M(B) R(W)
      if (left_sensor_state == BLACK       && middle_sensor_state == BLACK    &&  right_sensor_state == WHITE )
      {
        Serial.println("LINE FOLLOWING  going right L(B) M(B) R(W)");
         //advance(speed_R +8, speed_L);
         advance(speed_R , speed_L + plus_factor);
       
      }
      else
      {// opening else - left - left - stop - forward
        read_ir();
        // LEFT
        //     ||||   
        //   L  M R
        //     ||||
        // L(B) M(W) R(W)
        // <-
          if(left_sensor_state == BLACK     && middle_sensor_state == WHITE    && right_sensor_state == WHITE )
        {// opening if - go right
          Serial.println("LINE FOLLOWING going left L(B) M(W) R(W)");
          //advance(speed_R +8, speed_L );
          //advance(speed_R , speed_L + plus_factor);
          advance(speed_R  + plus_factor , speed_L);
           
          
        }// close if - go left
        else
        { // opening else - left - stop - forward
          read_ir();
          // L(W) M(B) R(B)
          //      | | |  
          //        L  
          //      | |  M 
          //      | |    R 
          // <-
          if (left_sensor_state == WHITE && middle_sensor_state == BLACK && right_sensor_state == BLACK  )
          { // opening if - going left
            Serial.println("LINE FOLLOWING turning right L(W) M(B) R(B) ");
             //advance(speed_R , speed_L +8);
             advance(speed_R + plus_factor, speed_L );
              
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
                  
                  state = TURN_TO_BIN;
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
