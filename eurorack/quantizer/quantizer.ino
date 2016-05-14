#include "Note.h";
#include "keyboard.h";

const int CV_IN = A0;
const int KEYBOARD_IN = A5;
const boolean DEBUG = true;
const int OCTAVE_RANGE = 5;

const int MIN_NOTE = 0;
const int MAX_NOTE = 251;

Note notes[12];
Keyboard* keyboard;
int notes_on_count = 12;
/*                          C # D # E F # G # A # B */
boolean MAJOR_SCALE[12]  = {1,0,1,0,1,1,0,1,0,1,0,1};
boolean MINOR_SCALE[12]  = {1,0,1,1,0,1,0,1,0,1,0,1};
boolean DORIAN_SCALE[12] = {1,0,1,1,0,1,0,1,0,1,1,0};

void set_scale(boolean scale[], int size = 12) 
{
  for(int i = 0; i < size; i++) 
  {
    notes[i].set_state(scale[i]);
  }
}


void setup()
{
  if (DEBUG) {
    Serial.begin(9600);          //  setup serial
  }
//  UCSR0B = 0;
  keyboard = new Keyboard(KEYBOARD_IN);
  
  for(int i = 0; i < 12; i++) {
    notes[i].set_led_pin(13 - i);
  }
  
  set_scale(MAJOR_SCALE);
  
}

int get_closest_note_offset(int note, int noteIndex) 
{
  if (notes[noteIndex].is_note_on()) {
    return note;
  }
  for (int i = 1; i < 12; i++)
  {

    if (note + i <= MAX_NOTE) { // If the next note up is in range
      int higher_index = noteIndex + i;
      if (higher_index >= 12) { // drop an octave if you've passed twelve (the returned note will still be higher)
        higher_index -= 12;
      }
      if (notes[higher_index].is_note_on()) {
        return note + i;
      }
    }

    if (note - i >= MAX_NOTE) { // If the next note up is in range
      int lower_index = noteIndex - i;
      if (lower_index < 0) { // increase an octave if you've passed zero (the returned note will still be lower)
        lower_index += 12;
      }
      if (notes[lower_index].is_note_on()) {
        return note - i;
      }
    }
 
  }
}

int loopCount = 0;
void loop()
{
  long cv_in_value         = analogRead(CV_IN);
  long octave_out_value    =  cv_in_value *  5 / 1024;
  long closest_note_index = (cv_in_value * 60 / 1024) % 12;
  long closest_note_number = closest_note_index + 60 * octave_out_value;
  long closest_note_on_number = get_closest_note_offset(closest_note_number, closest_note_index);

  if (loopCount >= 1000) {
    loopCount = 0;
  
    int key = keyboard->detect_key_down();
  
    if (key > -1) {
      notes_on_count += (notes[key].toggle_state() ? 1: -1);
      if (DEBUG) {
        Serial.print("cv_in_value:            ");   Serial.println(cv_in_value);
        Serial.print("octave_out_value:       ");   Serial.println(octave_out_value);
        Serial.print("closest_note_index:     ");   Serial.println(closest_note_index);
        Serial.print("closest_note_number:    ");   Serial.println(closest_note_number);
        Serial.print("closest_note_on_number: ");   Serial.println(closest_note_number);
        
        Serial.print("key pressed:            ");   Serial.println(key);
        Serial.print("notes_on_count:         ");   Serial.println(notes_on_count);
      }
    }
    
  } else {
    loopCount++;
  }
}

