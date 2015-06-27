// connect motor controller pins to Arduino digital pins
// motor one

#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int enA = 10; 
int in1 = 9;
int in2 = 8;
// motor two
int enB = 5;
int in3 = 7;
int in4 = 6;
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
int dir = -1;

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

  int dir = 0;

void loop()
{
  unsigned int dist = sample_dist_cm();
  if ( dist > 0 && dist <= 10) {
    dir = -1;
    move(-100, -100, 500);
    delay(500);
  } else if ( dir == -1 || (dist > 10 && dist <= 30 )) {
    dir = 0;
    move(dir * 200, dir * -200, 2000);
  } else {
    dir = 1;
    move(200, 200, dist * 10);
  }
}


void demoOne()
{
  // this function will run the motors in both directions at a fixed speed
  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, 200);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 200);
  delay(2000);
  // now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  delay(2000);
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void demoTwo()
{
  // this function will run the motors across the range of possible speeds
  // note that maximum speed is determined by the motor itself and the operating voltage
  // the PWM values sent by analogWrite() are fractions of the maximum speed possible 
  // by your hardware
  // turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  // accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++)
  {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i)
  {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  } 
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  
}

