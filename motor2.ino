//int led_pin = 13;
int IN4 = 13;
void setup() {
  Serial.begin(9600);
  pinMode(IN4, OUTPUT); 

  digitalWrite(IN4, LOW);
  
}
void loop() {

  digitalWrite(IN4, HIGH);
  delay(5000);
  digitalWrite(IN4, LOW);
  delay(500);
  
  if(Serial.available())                                   // if there is data comming
  {
    String command = Serial.readStringUntil('\n');         // read string until meet newline character
    if(command == "ON")
    {
//      digitalWrite(led_pin, HIGH);                         // turn on LED
      Serial.println("LED is turned ON");                  // send action to Serial Monitor
    }
    else if(command == "OFF")
    {
//      digitalWrite(led_pin, LOW);                          // turn off LED
      Serial.println("LED is turned OFF");                 // send action to Serial Monitor
    }
  }
}
