// connect motor controller pins to Arduino digital pins
// motor one

#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup()
{
  Serial.begin(9600); 
}

void loop()
{
  unsigned int dist = sonar.ping_cm();
  Serial.println(dist);
  delay(20);
   if ( dist > 0 && dist <= 10) {
    Serial.println("Back");
    delay(500);
  } else if ( dist > 10 && dist <= 30) {
    Serial.println("Turn");
  } else {
    Serial.println("Fwd");
  }  
}
