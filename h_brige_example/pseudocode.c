
/*
	Pseudocode for 7 digits of PI
	02/03/2020
*/


#define RIGHT 0
#define LEFT 1
#define motor_speed 100 // MAX = 255
#define turn_speed  85
#define BLACK 150
const float wheeldiameter = 70; // Wheel diameter in millimeters, change if different 
int turn_delay = 10;
volatile int counter_R = 0;
volatile int counter_L = 0;

//-----------------------
// Motor : Right
//-----------------------
int R_ME  = 3;   //Enable Pin of the Right Motor (must be PWM)
int R_M1 = 1;    //Control Pin

//-----------------------
// Motor : Left
//-----------------------
int L_ME  = 6;   //Enable Pin of the Left Motor (must be PWM)
int L_M1 = 4;


// ================================================================
// IR Sensor
// ================================================================

//-----------------------
// Sensor: Left
//-----------------------

const int left_sensor_pin  = A0;
int left_sensor_state;

//-----------------------
// Sensor: Right
//-----------------------

const int right_sensor_pin = A1;
int right_sensor_state;


void setup()
{
	// Increase counter A when speed sensor pin goes High
	attachInterrupt(digitalPinToInterrupt (MOTOR_R), ISR_countR, RISING);  
  	 // Increase counter B when speed sensor pin goes High
	attachInterrupt(digitalPinToInterrupt (MOTOR_L), ISR_countL, RISING); 
}


start() // initialize counter
time = time() // updated throughout all functions 
deadline =  const int 
position =  line count
s=stop() // if s = 1 return 


competition(){
	line_following(n = 2) // navigate over n white lines
	// going for 3
	turn(right) 	// rotate - 90 degrees
	stacking()
	{
		num_turns = go_to_block() 	// stop when block is at position for claw - return number of turns
		pick_block() 	// activate claw1, lead screw, claw2, lead screw, claw1
		drive_backwards( num_turns) // go back number of turns
	
	}
	position = 2

	if(t < deadline)
	{
		// 1
		turn(left)
		line_following(1)
		turn(left)
		stacking()
		position = 3
	}
	else 
		to_end(position,left)

	if(t < deadline)
	{	
		// 4
		turn(right)
		turn(right)
		stacking()
		position = 3
	}
	else 
		to_end(position,right)

	if(t < deadline)
	{	
		// 1
		turn(left)
		turn(left)
		stacking()
		position = 3
	}
	else 
		to_end(position,left )


	if(t < deadline)
	{	
		//5
		turn(right)
		line_following(n = 1)
		turn(right)
		stacking()
		position = 4
	}
	else 
		to_end(position,right )

	if(t < deadline)
	{	
		//9
		turn(left)
		turn(left)
		stacking()
		position = 4
	}
	else 
		to_end(position,left )

	if(t < deadline)
	{	
		//2
		turn(right)
		line_following(n = 1)
		turn(right)
		stacking()
		//END
		turn(left)
		line_following(n =1 )
	}
	else 
		to_end(position,left )

	num_turns = go_to_block() 	// stop when block is at position for claw - return number of turns
	put_stack_down()
	drive_backwards()
}


//==========================================================
// TURN
//==========================================================

int turn(int dir)
{
	if (dir == RIGHT ) // turn right
	{
		SpinLeft(CMtoSteps(19));

	}
	else 
	{
		SpinRight(CMtoSteps(19));
	}

	return 1;
}

// Motor A pulse count ISR
void ISR_countR()  
{
  counter_R++;  // increment Motor A counter value
} 

// Motor B pulse count ISR
void ISR_countL()  
{
  counter_L++;  // increment Motor B counter value
}


// Function to convert from centimeters to steps
int CMtoSteps(float cm) 
{
  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step
  float f_result = cm / cm_step;  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)  
  return result;  // End and return result
}


// Function to convert from centimeters to steps
int CMtoSteps(float cm) 
{
  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step
  float f_result = cm / cm_step;  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)  
  return result;  // End and return result
}

// Function to Spin Left
void SpinLeft(int steps) 
{
   counter_R = 0;  //  reset counter A to zero
   counter_L = 0;  //  reset counter B to zero
   while (steps > counter_R || steps > counter_L)
{ 
	// Set Motor A forward
	digitalWrite(R_M1, HIGH);
	analogWrite (R_ME,turn_speed);      //PWM Speed Control
	// Set Motor B reverse
	digitalWrite(L_M1, LOW);
	analogWrite (R_ME,turn_speed);      //PWM Speed Control
   }   
  // Stop when done
  digitalWrite(R_M1, LOW);
  // Set Motor B reverse
  digitalWrite(L_M1, LOW);
  
  counter_R = 0;  //  reset counter A to zero
  counter_L = 0;  //  reset counter B to zero 
}

// Function to Spin Right
void SpinRight(int steps) 
{
  Serial.println("Entering Right Function");
   counter_R = 0;  //  reset counter A to zero
   counter_L = 0;  //  reset counter B to zero
   while (steps > counter_R || steps > counter_L)
   {
   	// Set Motor A reverse
  	digitalWrite(R_M1, LOW);
	analogWrite (R_ME,turn_speed);      //PWM Speed Control
  	// Set Motor B forward
  	digitalWrite(L_M1, HIGH);
	analogWrite (L_ME,turn_speed);      //PWM Speed Control
 
   }
    
  // Stop when done
  digitalWrite(R_M1, LOW);
 // Set Motor B reverse
  digitalWrite(L_M1, LOW);

  counter_R = 0;  //  reset counter A to zero
  counter_L = 0;  //  reset counter B to zero 

}