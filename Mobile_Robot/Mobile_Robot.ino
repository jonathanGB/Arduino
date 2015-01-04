/* Project realized from January 2014 to March 2014 for a physics course-project at cégep by Jonathan Guillotte-Blouin (software and a little bit hardware) and Alexandre Larose (hardware) */

/* The robot has 2 independent DC engines, a distance sensor, a collision sensor, a quadruple half h-bridge (L293D) and an Arduino Uno board */

/* Its goal is to turn on itself until it finds a target in a radius of 70 cm (the distance sensor limit), make sure it indeed is an object, then go towards it, then stop if front of it
   when a collision is detected by the collision sensor. */
   
/* When testing, the Arduino was alimented by a USB 2.0 cable connected to the computer, and it would communicate via a serial monitor displayed in the Arduino IDE many information that we 
   found helpful. */
   
/* This was done with little prior experience in programming (only a little bit of JavaScript at the time) and in hands-on electronics. It mixes programming, theoretical and practical knowledge
   of electricity and a lot of research. */


#define leftF 5 // left engine forwards
#define rightF 9   
#define leftB 3 // left engine backwards
#define rightB 6
#define enableR 8 // activate the right engine
#define enableL 2 // activate the left engine

#define distance A3 // position of the distance sensor
#define collision A0 // position of the collision sensor

#define red 10 // position of the red LED
#define green 11 // position of the green LED
#define blue 12 // position of the blue LED

unsigned long timeD = 0; // used to blink the LED
unsigned long timeT = 0; // used to wait for the "turn" function
boolean state = 0; // gives the LED state (HIGH or LOW)
unsigned long count = 0; // count the number of times the "loop" has been called

byte i = 0;
const byte n = 5; // amount of data capture per filtered packet
int tensionAverage[n]; // array containing the n last distance mesures, which will later be filtered
unsigned int totalAverage = 0;
unsigned int totalMedian = 0;

void setup() {
  // engines
  pinMode(leftF, OUTPUT); // announce the different inputs and outputs connected to the Arduino
  pinMode(rightF, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(rightB, OUTPUT);
  pinMode(enableL,OUTPUT);
  pinMode(enableR,OUTPUT);

  // sensors
  pinMode(distance,INPUT);
  pinMode(collision,INPUT);

  // LED
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);

  Serial.begin(9600); // start a communication with the computer at 9600 baud
}


/* Function to turn the robot until an object is detected */
void turn () 
{

  while (true)
  {
      averageMedian();

    if (totalMedian < 90) // if no object is detected, make the robot turn on itself, and turn on the red LED
    {
      // engines
      digitalWrite(enableL,HIGH);
      digitalWrite(enableR,HIGH); 
      analogWrite(leftF,0);
      analogWrite(rightB,0);
      analogWrite(leftB,40);  
      analogWrite(rightF,40);

      // LED
      digitalWrite(green,LOW);
      digitalWrite(blue,LOW);
      digitalWrite(red,HIGH);
    }  

    else // if not, stop the engines for 1.5 second and initiate a LED chaser
    { 
      timeT = millis(); 

      while (millis() - timeT < 1500)
      { 
      digitalWrite(enableL,LOW);
      digitalWrite(enableR,LOW);
        if (millis() - timeT < 250)
        {
          digitalWrite(blue,LOW);
          digitalWrite(red,LOW);
          digitalWrite(green,HIGH);
        }
        else if (millis() - timeT < 500)
        {
          digitalWrite(green,LOW);
          digitalWrite(red,LOW);
          digitalWrite(blue,HIGH); 
        }
        else if(millis() - timeT < 750)
        {
          digitalWrite(green,LOW);
          digitalWrite(blue,LOW);
          digitalWrite(red,HIGH); 
        }
        else if (millis() - timeT < 1000)
        {
          digitalWrite(blue,LOW);
          digitalWrite(red,LOW);
          digitalWrite(green,HIGH); 
        }
        else if (millis() - timeT < 1250)
        {
          digitalWrite(green,LOW);
          digitalWrite(red,LOW);
          digitalWrite(blue,HIGH); 
        }
        else if (millis() - timeT < 1500)
        {
          digitalWrite(green,LOW);
          digitalWrite(blue,LOW);
          digitalWrite(red,HIGH); 
        }
      } 

      averageMedian();

      if (totalMedian > 90) // if after 1.5 second an object is still detected, then stop turning
        break;
    }
  }
}


/* Function for the conditional alimentation of the engines */
void engine(int tensionDistance, int tensionCollision) {
  if (tensionCollision > 650){ // if there is a collision with the target, stop the engines
    digitalWrite(enableL,LOW);
    digitalWrite(enableR,LOW);
  }

  else if (tensionDistance>250) { // if the target is near, slow the engines
    digitalWrite(enableL,HIGH);
    digitalWrite(enableR,HIGH);  
    analogWrite(leftB,0);
    analogWrite(rightB,0);
    analogWrite(leftF,57);  
    analogWrite(rightF,50);
  }

  else { // if not, go to a cruising speed
    digitalWrite(enableL,HIGH);
    digitalWrite(enableR,HIGH);
    analogWrite(leftB,0);
    analogWrite(rightB,0);
    analogWrite(leftF,78);  
    analogWrite(rightF,70);

  }

}


/* Function for the lightning of the LEDs */
void LED(int tensionDistance, int tensionCollision) {

  state = !state;  
  if(tensionCollision > 650) { // if the robot has collided, blink the green LED
    digitalWrite(red,LOW);
    digitalWrite(blue,LOW);
    digitalWrite(green,state);
  }

  else if(tensionDistance > 250) { // if the robot is close to the target, blink the blue LED
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,state);
  } 

  else { // if not, blink the red LED
    digitalWrite(green,LOW);               
    digitalWrite(blue,LOW);
    digitalWrite(red,state);
  } 
}


/* Function to filter data coming from the distance sensor, and communicate the results to the computer */
void averageMedian() {
  totalAverage = 0;
  totalMedian = 0;
  Serial.print(" Array: [");

  for (i=0; i<n; i++) {
    tensionAverage[i] = analogRead(distance);
    totalAverage = totalAverage + tensionAverage[i];
    Serial.print(tensionAverage[i]);

    if(i != n-1) {
      Serial.print(", ");
    }

    else {
      Serial.println("]"); 
    }
  }
  totalMedian = totalAverage;
  totalAverage = totalAverage/n;


  int excluMin = min(tensionAverage[0], min(tensionAverage[1], min(tensionAverage[2], min(tensionAverage[3], tensionAverage[4]))));
  int excluMax = max(tensionAverage[0], max(tensionAverage[1], max(tensionAverage[2], max(tensionAverage[3], tensionAverage[4]))));
  int excluded = excluMin + excluMax;

  totalMedian = (totalMedian - excluded)/(n-2);

  Serial.print(" totalAverage: ");
  Serial.println(totalAverage);
  Serial.print("excluMin: ");
  Serial.println(excluMin);
  Serial.print("excluMax: ");
  Serial.println(excluMax);
  Serial.print("totalMedian: ");
  Serial.println(totalMedian);
  Serial.println("");
}

void loop() {
  if (count == 0) // we only want to call turn() once (at the beginning)
    turn();

  averageMedian();
  unsigned int lectureD = totalMedian;
  unsigned int lectureC = analogRead(collision); 
  Serial.print("Collision: ");
  Serial.println(lectureC);

  engine (lectureD, lectureC);

  if ((millis() - timeD) > 100) { // blink every 100 ms
    LED(lectureD, lectureC);
    timeD = millis();                             
  }

  count++;
}