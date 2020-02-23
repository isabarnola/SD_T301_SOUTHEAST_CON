int sR = 2;
int sM = 3;
int sL = 4;

int R;
int M;
int L;

void ir_isr()
{
 R = digitalRead(sR);
 M = digitalRead(sM);
 L = digitalRead(sL);
}

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(0, ir_isr, CHANGE);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(L);
  Serial.print("\t");
  Serial.print(M);
   Serial.print("\t");
  Serial.println(R);

}
