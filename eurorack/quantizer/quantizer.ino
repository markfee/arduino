#include "Note.h";
#include "keyboard.h";

int analogPin = 0;
int buttonIn = 5;
int val = 0;           // variable to store the value read

Note notes[12];
Keyboard* keyboard;

void setup()
{
  Serial.begin(9600);          //  setup serial
//  UCSR0B = 0;
  keyboard = new Keyboard(A5);
  
  for(int i = 0; i < 10; i++) {
    notes[i].set_led_pin(i+2);
  }
}


void loop()

{
  val = analogRead(analogPin);    // read the input pin
  
  int key = keyboard->detect_key_down();
  if (key > 0) {
    notes[key-1].toggle_state();
    Serial.println(key);             // debug value
  }
  return;

  digitalWrite(0, HIGH);   // sets the LED on
  delay(1000);                  // waits for a second
  digitalWrite(0, LOW);    // sets the LED off
  delay(1000);                  // waits for a second  
  
  return;
  
//  if (!note->read_button_state()) {
//    Serial.print(val);             // debug value
//    Serial.print(": ");             // debug value
    int note = round((0.0 + val) / 17);
//    Serial.print(note);             // debug value
    
//    Serial.print(": ");             // debug value
//    Serial.println(note_c->is_note_on());             // debug value
    delay(100);
//  }
}

