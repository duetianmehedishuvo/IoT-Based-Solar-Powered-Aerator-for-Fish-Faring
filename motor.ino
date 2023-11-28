int ENB = D2;
int IN3 = D1;
int IN4 = D0;

void setup() {
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  setDirection();
  delay(1000);
  changeSpeed();
  delay(1000);
}

void setDirection() {
  analogWrite(ENB, 255);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(5000);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(5000);
  
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void changeSpeed() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  for (int i = 0; i < 256; i++) {
    analogWrite(ENB, i);
    delay(20);
  }
  
  for (int i = 255; i >= 0; --i) {
    analogWrite(ENB, i);
    delay(20);
  }
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
