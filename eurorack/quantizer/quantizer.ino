#include "Note.h";
#include "keyboard.h";

const int CV_IN = A0;
const int CV_OUT = A1;
const int KEYBOARD_IN = A5;
const boolean DEBUG = true;
const int OCTAVE_RANGE = 5;

const int MIN_NOTE = 0;
const int MAX_NOTE = 251;

Note notes[12];
Keyboard* keyboard;
int notes_on_count = 12;
/*                          C # D # E F # G # A # B */
char*   NOTE_NAMES[12]  = {"C ","C#","D ","D#","E ","F ","F#","G","F#","A ","A#","B "};
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
  pinMode(CV_OUT, OUTPUT);

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

int get_closest_note_offset(int note, int noteIndex, boolean debug = false) 
{
  if (notes[noteIndex].is_note_on()) {
    if (debug) {
      Serial.println("Note is ON!");
    }
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
        if (debug) {
          Serial.print("Returning HIGHER NOTE "); Serial.println(note + i);
        }
        return note + i;
      }
    }

    if (note - i >= MIN_NOTE) { // If the next note up is in range
      int lower_index = noteIndex - i;
      if (lower_index < 0) { // increase an octave if you've passed zero (the returned note will still be lower)
        lower_index += 12;
      }
      if (notes[lower_index].is_note_on()) {
        if (debug) {
          Serial.print("Returning LOWER NOTE "); Serial.println(note - i);
        }
        return note - i;
      }
    }
  }
  return -1;
}

int loopCount = 0;
void loop()
{
  int key = -1;
  // Onlyt Read the keyboard one in 1000 cycles to free up the processor to process CV
  if (0 == (loopCount % 1000)) {
    key = keyboard->detect_key_down();
    if (key > -1) {
      notes_on_count += (notes[key].toggle_state() ? 1: -1);
    }  
  }
  
  long cv_in_value                = analogRead(CV_IN);
  if (cv_in_value > 0 && cv_in_value < 275) {
    // OFFSET the low values to compensate for digital errors with low values
    cv_in_value +=5;
  }
  long octave_in_value            =  cv_in_value *  5 / 1024;
  long closest_note_index         = (cv_in_value * 60 / 1024) % 12;
  long closest_note_number        = closest_note_index + 12 * octave_in_value;
  long closest_note_number2       = cv_in_value * 60 / 1024;
  long closest_note_on_number     = get_closest_note_offset(closest_note_number, closest_note_index);
  long octave_out_value           = closest_note_on_number / 60;
  long closest_note_on_index      = closest_note_on_number % 12;
//  bits_per_note = 1024 / 60
  long cv_out_value               = closest_note_on_number * 1024 / 60;
  analogWrite(CV_OUT, (loopCount % 1024));
  analogWrite(CV_OUT, cv_out_value);
  
//  analogWrite(CV_OUT, 250);

  if (loopCount >= 10000 || key > -1) {
    loopCount = 0;
    if (DEBUG) {
      closest_note_on_number = get_closest_note_offset(closest_note_number, closest_note_index, DEBUG);
      Serial.print("cv_in_value:            ");   Serial.println(cv_in_value);
      Serial.print("cv_out_value:           ");   Serial.println(cv_out_value);

      Serial.print("octave_in_value:        ");   Serial.println(octave_in_value);
      Serial.print("closest_note_index:     ");   Serial.print(NOTE_NAMES[closest_note_index]);        Serial.println(octave_in_value);
      Serial.print("closest_note_on_index:  ");   Serial.print(NOTE_NAMES[closest_note_on_index]);     Serial.println(octave_out_value);
      
      Serial.print("closest_note_number:    ");   Serial.println(closest_note_number);
      Serial.print("closest_note_number2:   ");   Serial.println(closest_note_number2);
      Serial.print("closest_note_on_number: ");   Serial.println(closest_note_on_number);
      
      Serial.print("key pressed:            ");   Serial.println(key);
      Serial.print("notes_on_count:         ");   Serial.println(notes_on_count);
    }
  } else {
    loopCount++;
  }
}

