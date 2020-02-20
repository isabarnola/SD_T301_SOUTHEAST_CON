

/*Contains movement functions left, right, forward*/

volatile long int global_ticks = 0;
volatile int      move_status  = 0;
volatile int      move_command = 0;  /* 0 - stop 1 - left turn; 2- forward ; 3 - right turn ; 4 u turn  */

/*function prototypes*/

void    wait_until_move_done(void);
void    left_turn(void);
void    right_turn(void);
void    forward(void);
void    set_move_command(int command);

void    move_mr(int mc);
void    stop_mr(void);


/*vel correction is intended to compensate misaplignment of the wheels*/

void vel_correction(void)
{
  des_wvel[0] = 1.0*des_wvel[0];
  des_wvel[1] = 1.0*des_wvel[1]; 
}


/* The turn_time and f_time are specific to a robot. 
   f_time     - move one grid
   turn_time  - turn 90 deg.
   Change/calibrate them for your robot */
   
int      turn_time    = 160;
int      f_time       = 450;


void left_turn(void)
{
    
   
  if (global_ticks < turn_time)
       {
        des_wvel[0] = -0.25;    
        des_wvel[1] =  0.25; 
       }
    
   else 
   {
     /* stop  */
     des_wvel[0]  =   0.;
     des_wvel[1]  =   0.;
     /*done moving*/
     move_status  =  0; 
    
    }

    vel_correction();
   
}


void right_turn(void)
{
    
   
  if (global_ticks < turn_time)
       {
        des_wvel[0] =   0.25;
        des_wvel[1] =  -0.25; 
       }
    
   else 
   {
     /* stop  */
     des_wvel[0]  =   0.;
     des_wvel[1]  =   0.;
     /*done moving*/
     move_status  =  0; 
    
    }

     vel_correction();
   
}


void forward(void)
{
   
  if (global_ticks < f_time)
       {
        des_wvel[0] =   0.25;
        des_wvel[1] =   0.25; 
       }
    
   else 
   {
     /* stop  */
     des_wvel[0]  =   0.;
     des_wvel[1]  =   0.;
     /*done moving*/
     move_status  =  0; 
    
    }

 vel_correction();
 
}
void stop_mr(void)
{   
    
   if (global_ticks < turn_time)
       {
        des_wvel[0] =   0.;
        des_wvel[1] =   0.; 
       }
    
   else 
   {
     move_status  =  0; 
    }
    
}



void move_mr(int mc)
{
 
   
switch(mc)
 {
   case 0: 
     stop_mr();
     break;
     
  case 1: 
     left_turn();
     break;
     
     case 2:
       forward();
     break; 
     
   case 3: 
     right_turn();
     break;
     
    default: 
     stop_mr();
 
 }
 
}



  

void set_move_command(int command)
{
 move_command = command;
 global_ticks = 0;
 move_status  = 1;
}



void wait_until_move_done(void)
{
  
  while(move_status != 0)
  {
   //Serial.print("");
  }

}


/*

#define front_sensor      0 

#define left_sen          0
#define front_sen         1
#define right_sen         2





float   dist_sensor[3] = {};

void read_sensor(void)
{
  int i;
  long int temp_sensor=0;
 
 set_move_command(1);
 wait_until_move_done(); 

        temp_sensor = 0; 
       for(i=0; i < 50; i++)
       {
         temp_sensor += analogRead(front_sensor); 
         delay(10);
       }

        dist_sensor[0] = temp_sensor/50.;

        Serial.print("L Sen: ");
        Serial.println(dist_sensor[0]);

  delay(1000);
   
 set_move_command(3);
 wait_until_move_done(); 

 temp_sensor = 0; 
 for(i=0; i < 50; i++)
 {
   temp_sensor += analogRead(front_sensor); 
   delay(10);
 }

  dist_sensor[1] = temp_sensor/50.;

  
  Serial.print("F Sen: ");
  Serial.println(dist_sensor[1]);

  delay(1000);

  set_move_command(3);
  wait_until_move_done(); 

  temp_sensor = 0; 
  for(i=0; i < 50; i++)
  {
    temp_sensor += analogRead(front_sensor); 
    delay(10);
  }
  
   Serial.print("R Sen: ");
   dist_sensor[2] = temp_sensor/50.;

   
   Serial.println(dist_sensor[2]);

   delay(1000);
   
  set_move_command(1);
  wait_until_move_done();   

}

*/









