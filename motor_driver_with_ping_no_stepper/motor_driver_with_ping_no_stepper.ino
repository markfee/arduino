// connect motor controller pins to Arduino digital pins
// motor one

#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

int in1 = 8;
int in2 = 9;
// motor two
int in3 = 10;
int in4 = 11;
//PWM -- these need to be on PWM enabled pins!
int enA = 4; 
int enB = 5;

#define LEFT_BUMPER_PIN 2
#define RIGHT_BUMPER_PIN 3

bool leftObstacleDetected = false;
bool rightObstacleDetected = false;

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

  pinMode(LEFT_BUMPER_PIN, INPUT);
  pinMode(RIGHT_BUMPER_PIN, INPUT);
  digitalWrite(LEFT_BUMPER_PIN, HIGH); // Turn on pullup resistor
  digitalWrite(RIGHT_BUMPER_PIN, HIGH); // Turn on pullup resistor

  attachInterrupt(0, leftObstacle, FALLING);
  attachInterrupt(1, rightObstacle, FALLING);
}

void leftObstacle() {
    if (leftObstacleDetected) 
      return;
    leftObstacleDetected = true;  
    Serial.println("\nLeft Obstacle");
    stop();  
}

void rightObstacle() {
    if (rightObstacleDetected) 
      return;
    rightObstacleDetected = true;
    Serial.println("\nRight Obstacle");
    stop();  
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

int dir = 1;
boolean doMove = true;
char chbuffer[64];


void move(int left, int right, int time) 
{
  sprintf(chbuffer, "Moving %d, %d", left, right);
//  Serial.println(chbuffer); 
  if (!doMove) {
    return;
  }
//  left *= -1;
  right *= -1;
  digitalWrite(in1, left > 0 ? HIGH : LOW);
  digitalWrite(in2, left < 0 ? HIGH : LOW);

  digitalWrite(in3, right > 0 ? HIGH : LOW);
  digitalWrite(in4, right < 0 ? HIGH : LOW);
  setSpeed(abs(left), abs(right));
  
  delay(time);
  
  stop();
}

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

unsigned int distFwd;
unsigned int distLeft;
unsigned int distRight;
unsigned int distAvg;
unsigned int distMin;

void loop()
{    
    if (leftObstacleDetected || rightObstacleDetected) {
      if (leftObstacleDetected && rightObstacleDetected) {
        Serial.print("X");
        move(-200, -200, 500);
      } else if (leftObstacleDetected) {
        Serial.print("<");
        move(-200, -200, 250);
        move(200, -200, 1000);
      } else if (rightObstacleDetected) {
        Serial.print(">");
        move(-200, -200, 250);
        move(-200, 200, 1000);
      }
      leftObstacleDetected = false; 
      rightObstacleDetected = false;
      delay(250);
      return;
    }
    distFwd = sample_dist_cm(); 


    if (distFwd > 30) {
      Serial.print("+");
      move(200, 200, distFwd * 10);
    } 
    else if (distFwd > 10) {
      Serial.print("O");
      move(200, -200, 1000);
    } else {
      Serial.print("-");
      move(-200, -200, 500);
    }

  return;

}
