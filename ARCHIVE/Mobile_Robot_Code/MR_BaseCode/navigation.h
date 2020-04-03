
#include "my_func.h"


volatile int mr_direction = 0;
volatile int mr_pos       = 0; 

int x_pos        = 0;
int y_pos        = 0; 
int obs_pos[16]  = {0};       

#define obs_threshold 300
/*set move command*/
/* 0 -stop*/
/* 1 - left */
/* 2 - forward*/
/* 3-  right*/


void navigation(void)
{
 
 switch(mr_direction)
 {
    case 0:
           read_sensor();
           Serial.println("Case 0");
           
           if ((mr_pos%4 > 0) && (dist_sensor[right_sen]< obs_threshold))
           {  
              set_move_command(3);
              wait_until_move_done();   
              set_move_command(2);
              wait_until_move_done();   
              
              mr_direction  = 3;
              mr_pos        = mr_pos - 1;    
           }

           else if ((mr_pos/4 <3) && (dist_sensor[front_sen]< obs_threshold))
           {
               
              set_move_command(2);
              wait_until_move_done();   
              mr_direction  = 0;
              mr_pos        = mr_pos + 4;    
           }       

           else if ((mr_pos%4 <3) && (dist_sensor[left_sen]< obs_threshold))
           {

              set_move_command(1);
              wait_until_move_done(); 
              set_move_command(2);
              wait_until_move_done();   
              
              mr_direction  = 1;
              mr_pos        = mr_pos + 1;  
            
           }
           else 
           {
             set_move_command(1);
             wait_until_move_done(); 
             set_move_command(1);
             wait_until_move_done(); 

             set_move_command(2);
             wait_until_move_done();
             
              mr_direction  = 2;
              mr_pos        = mr_pos - 4;          
           
           }
     
    break;

    case 1:
        stop_mr(); 
 
    break; 

    default: 
        stop_mr();
 }

           Serial.print("Dir :  ");
           Serial.println(mr_direction);
           Serial.print("Pos :");
           Serial.println(mr_pos);
           

}


