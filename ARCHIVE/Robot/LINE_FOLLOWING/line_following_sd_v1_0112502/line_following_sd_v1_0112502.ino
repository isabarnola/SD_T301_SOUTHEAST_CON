/*
  FILE :  line_following_ds_v1_0112502
  DESCRIPTION: SIMPLE LINE FOLLOWING ROBOT
  AUTHOR : ISABEL BARNOLA
  REFERENCES:
    https://create.arduino.cc/projecthub/robocircuits/line-follower-robot-arduino-299bae

    USING SENSOR ANALOG READ 
    EXAMPLE USES WHITE SURFACE AND BLACK LINES
    TEST CURRENT SET UP AND THEN CHANGE VALUES
*/

//Define Pins

int motor_A_en = 3; //Enable Pin of the Right Motor (must be PWM)
int motor_A_in1 = 1; //Control Pin
int motor_A_in2 = 2;

int motor_B_en = 6; //Enable Pin of the Left Motor (must be PWM)
int motor_B_in1 = 4;
int motor_B_in2 = 5;

//Speed of the Motors
#define ENASpeed 100 
#define ENBSpeed 100

int right_sensor_state = 0;
int Sensor2 = 0;
int Sensor3 = 0;
int left_sensor_state = 0;

void setup() {

  pinMode(motor_A_en, OUTPUT);
  pinMode(motor_A_in1, OUTPUT);
  pinMode(motor_A_in2, OUTPUT);

  pinMode(motor_B_en, OUTPUT);
  pinMode(motor_B_in1, OUTPUT);
  pinMode(motor_B_in2, OUTPUT);

  pinMode(right_sensor_state, INPUT);
  pinMode(Sensor2, INPUT);
  pinMode(Sensor3, INPUT);
  pinMode(left_sensor_state, INPUT);
  
}

void loop()
{
  //Use analogWrite to run motor at adjusted speed
  analogWrite(motor_A_en, ENASpeed);
  analogWrite(motor_B_en, ENBSpeed);

  //Read the Sensor if HIGH (BLACK Line) or LOW (WHITE Line)
  /*
  Sensor1 = digitalRead(8);
  Sensor2 = digitalRead(9);
  Sensor3 = digitalRead(10);
  Sensor4 = digitalRead(11);
  */

  int rigth_sensor_state = digitalRead(8);
  int left_sensor_state =  digitalRead(9);
  //Set conditions for FORWARD, LEFT and RIGHT


  //RIGHT
  if (left_sensor_state == LOW &&  right_sensor_state == HIGH)
  {

    //Turn RIGHT
    //motor A Forward
    digitalWrite(motor_A_in1, HIGH);
    digitalWrite(motor_A_in2, LOW);

    //motor B Backward
    digitalWrite(motor_B_in1, LOW);
    digitalWrite(motor_B_in2, HIGH);
  }
  
  // LEFT
  else if(left_sensor_state == HIGH && right_sensor_state == LOW)
  {

    //Turn LEFT
    //motor A Backward
    digitalWrite(motor_A_in1, LOW);
    digitalWrite(motor_A_in2, HIGH);

    //motor B Forward
    digitalWrite(motor_B_in1, HIGH);
    digitalWrite(motor_B_in2, LOW);
  }
  
  //FORWARD
  else if(left_sensor_state == HIGH && right_sensor_state == HIGH)
  {
    //if(Sensor4 == LOW && Sensor3 == HIGH && Sensor2 == HIGH && Sensor1 == LOW
    digitalWrite(motor_A_in1, LOW);
    digitalWrite(motor_A_in2, HIGH);
    digitalWrite(motor_B_in1, LOW);
    digitalWrite(motor_B_in2, HIGH);
  }
  // STOP
  else
  {
      analogWrite(motor_A_en, 0);
      analogWrite(motor_B_en, 0);
    
  }
}
