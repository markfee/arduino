// connect motor controller pins to Arduino digital pins
// motor one

#include <NewPing.h>

#define TRIGGER_PIN  11  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int enA = 10; 
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;

int motorPin1 = 2;
int motorPin2 = 3;
int motorPin3 = 4;
int motorPin4 = 5;

int motorSpeed = 10000;  //variable to set stepper speed
int count = 0;          // count of steps made
int countsperrev = 128; // number of steps per full revolution
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};



void setup()
{
  Serial.begin(9600); 
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  //declare the motor pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);  
  char buffer[32];
  for (int i = 5; i > 0; i--) {
    sprintf(buffer, "Please wait %d seconds\n", i);
    Serial.print(buffer);
    delay(1000);
  }
}

void setSpeed(int left, int right) 
{
  analogWrite(enA, left);
  analogWrite(enB, right);
}

void stop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void move(int left, int right, int time) 
{
  left *= -1;
  right *= -1;
  digitalWrite(in1, left > 0 ? HIGH : LOW);
  digitalWrite(in2, left < 0 ? HIGH : LOW);

  digitalWrite(in3, right > 0 ? HIGH : LOW);
  digitalWrite(in4, right < 0 ? HIGH : LOW);
  setSpeed(abs(left), abs(right));
  
  delay(time);
  
  stop();
}
char chbuffer[32];

unsigned int sample_dist_cm() {
  const unsigned int PING_COUNT = 6;
  unsigned int dist = 0; 
  unsigned int zero_count = 0;
  
  for (int i = 0; i < PING_COUNT; i++) {
    unsigned int ping = sonar.ping_cm();
    if (ping ==0) {
      zero_count++;
    } else {
      dist += ping;
    }
  }
  return zero_count > (PING_COUNT /  2) ? 100 :  (unsigned int) ((0.5 + dist) / (PING_COUNT - zero_count ) );
}

  int dir = 1;
  boolean doMove = false;
void loop()
{
  unsigned int dist = sample_dist_cm();
  Serial.println(count);
  
  if(count < countsperrev ) {
    Serial.println("clockwise");
    clockwise();
  }
  else if (count == countsperrev * 2) {
    count = 0;
  }
  else {
    Serial.println("anticlockwise");
    anticlockwise();
  }
  count++;  
  
  if (doMove) {
    if ( dist > 0 && dist <= 10) {
      dir = -1;
      move(-100, -100, 500);
      delay(500);
    } else if ( dir == -1 || (dist > 10 && dist <= 50 )) {
      dir = 0;
      move(200, -200, 2000);
    } else {
      dir = 1;
      move(200, 200, dist * 10);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)
void anticlockwise()
{
  for(int i = 0; i < 8; i++)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void clockwise()
{
  for(int i = 7; i >= 0; i--)
  {
    setOutput(i);
    delayMicroseconds(motorSpeed);
  }
}

void setOutput(int out)
{
  digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
}

