
//-----------------------
// Sensor: Left
//-----------------------

const int ap  = A0;
int analog;

//-----------------------
// Sensor: Right
//-----------------------

const int dp = 8;
int digital;

void setup() {
  // put your setup code here, to run once:
   pinMode(ap,    INPUT);
   pinMode(dp, INPUT);
   Serial.begin(9600); 

}

void loop() {
  // put your main code here, to run repeatedly:
  analog =  analogRead(ap);
  digital = digitalRead(dp);
  Serial.print(analog);
  Serial.print('\t');
  Serial.print(digital);
  Serial.print('\n');

}
