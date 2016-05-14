#include "Note.h";
#include "keyboard.h";

const int CV_IN = A0;
const int KEYBOARD_IN = A5;
const boolean DEBUG = true;
const int SERIAL_OFFSET = (DEBUG ? 2 : 0);
const int OCTAVE_RANGE = 5;

Note notes[12];
Keyboard* keyboard;
int notes_on_count = 12;
/*                          C # D # E F # G # A # B */
boolean MAJOR_SCALE[12]  = {1,0,1,0,1,1,0,1,0,1,0,1};
boolean MINOR_SCALE[12]  = {1,0,1,1,0,1,0,1,0,1,0,1};
boolean DORIAN_SCALE[12] = {1,0,1,1,0,1,0,1,0,1,1,0};


void setup()
{
  if (DEBUG) {
    Serial.begin(9600);          //  setup serial
  }
//  UCSR0B = 0;
  keyboard = new Keyboard(KEYBOARD_IN);
  
  for(int i = 0; i < 12 - SERIAL_OFFSET; i++) {
    notes[i].set_led_pin(i + SERIAL_OFFSET);
  }
  
}

int loopCount = 0;
void loop()
{
  long cv_in_value         = analogRead(CV_IN);
  long octave_out_value    =  cv_in_value *  5 / 1024;
  long closest_note_number = (cv_in_value * 60 / 1024) % 12;

  if (loopCount >= 1000) {
    loopCount = 0;
  
    int key = keyboard->detect_key_down();
  
    if (key > -1) {
      notes_on_count += (notes[key].toggle_state() ? 1: -1);
      if (DEBUG) {
        Serial.print("cv_in_value:         ");   Serial.println(cv_in_value);
        Serial.print("octave_out_value:    ");   Serial.println(octave_out_value);
        Serial.print("closest_note_number: ");   Serial.println(closest_note_number);
        Serial.print("key pressed:         ");   Serial.println(key);
        Serial.print("notes_on_count:      ");   Serial.println(notes_on_count);
      }
    }
    
  } else {
    loopCount++;
  }
  
  return;

  digitalWrite(0, HIGH);   // sets the LED on
  delay(1000);                  // waits for a second
  digitalWrite(0, LOW);    // sets the LED off
  delay(1000);                  // waits for a second  
  
  return;
  
//  if (!note->read_button_state()) {
    int note = round((0.0 + cv_in_value) / 17);
      if (DEBUG) {
        Serial.print(cv_in_value);             // debug value
        Serial.print(": ");             // debug value
        Serial.print(note);             // debug value
        Serial.print(": ");             // debug value
//        Serial.println(note_c->is_note_on());             // debug value
        delay(100);
      }
//  }
}

