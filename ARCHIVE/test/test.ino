
void setup() {
 
  Serial.begin(9600);  // For serial print (debugging)

}
int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(i);
  i = i + 1;
}