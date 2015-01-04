// must be a PWM on the Arduino Board
#define led 5
 
int voltage = 0;
 
void setup() {
  pinMode(led, OUTPUT);
}
 
void turnOn() {
  if (voltage != 255)
  {
     analogWrite(led, voltage); 
     delay(5); // in ms
     voltage++;
     turnOn();
  } 
}
 
void turnOff() {
  if (voltage != 0)
  {
     analogWrite(led, voltage);
     delay(5); // in ms
     voltage--;
     turnOff();
  } 
}
 
void loop() {
  turnOn();
  turnOff();
}