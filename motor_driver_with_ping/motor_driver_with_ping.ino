// connect motor controller pins to Arduino digital pins
// motor one

#include <NewPing.h>
#include <Stepper.h>


#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

/*-----( Declare Constants, Pin Numbers )-----*/
//---( Number of steps per revolution of INTERNAL motor in 4-step mode )---
#define STEPS_PER_MOTOR_REVOLUTION 32   

//---( Steps per OUTPUT SHAFT of gear reduction )---
#define STEPS_PER_OUTPUT_REVOLUTION 32 * 64  //2048  

/*-----( Declare objects )-----*/
// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to

//The pin connections need to be 4 pins connected
// to Motor Driver In1, In2, In3, In4  and then the pins entered
// here in the sequence 1-3-2-4 for proper sequencing
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 2, 4, 3, 5);



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

int motorSpeed = 800;  //variable to set stepper speed
int count = 0;          // count of steps made
int countsperrev = 128; // number of steps per full revolution
//int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
int lookup[8] = {B00001, B00011, B00010, B00110, B00100, B01100, B01000, B01001};



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
  for (int i = 3; i > 0; i--) {
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

int dir = 1;
boolean doMove = true;
char chbuffer[64];


void move(int left, int right, int time) 
{
  if (!doMove) {
    sprintf(chbuffer, "Moving %d, %d", left, right);
    Serial.println(chbuffer); 
    return;
  }
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
  do {
    distFwd = sample_dist_cm(); 
    if (distFwd > 50) {
      move(200, 200, distFwd * 10);
    } 
  } while (distFwd > 50); 
  
  Serial.println("clockwise");
  clockwise();
  distLeft = sample_dist_cm();
  anticlockwise();

  Serial.println("anticlockwise");
  anticlockwise();
  distRight = sample_dist_cm();
  clockwise();
  
  distAvg = (unsigned int) ( ( (distFwd + distLeft + distRight) / 3 )  + 0.5 );
  distMin = distLeft < distRight ? distLeft : distRight;
  
  sprintf(chbuffer, "distLeft: %d, distRight: %d, distAvg: %d, distMin: %d", distLeft, distRight, distAvg, distMin);
  Serial.println(chbuffer);
  if (distAvg > 0 && distAvg <= 10) { // Something Ahead
    dir = -1;
    move(-100, -100, 500);
    delay(500);
  } else if ( dir == -1 || (distMin > 10 && distMin <= 50 )) {
    dir = 0;
    int objectLeft = distLeft < distRight ? -1 : 0;
    int objectRight = -(objectLeft+1);

    move(objectLeft * 200, objectRight * 200, 1000);
  } else {
    dir = 1;
    move(200, 200, distAvg * 10);
  }
}
//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
//delay "motorSpeed" between each pin setting (to determine speed)

void clockwise()
{
  small_stepper.setSpeed(100);   
  small_stepper.step(STEPS_PER_OUTPUT_REVOLUTION / 12);  
}

void anticlockwise()
{
  small_stepper.setSpeed(100);   
  small_stepper.step(- STEPS_PER_OUTPUT_REVOLUTION / 12);  
}


void setOutput(int out)
{
  digitalWrite(motorPin1, bitRead(lookup[out], 0));
  digitalWrite(motorPin2, bitRead(lookup[out], 1));
  digitalWrite(motorPin3, bitRead(lookup[out], 2));
  digitalWrite(motorPin4, bitRead(lookup[out], 3));
}

