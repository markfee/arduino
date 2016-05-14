#include "Note.h"

int Note::note_count = 0;

Note::Note()
{
  this->pin = pin;
  b_note_on = true;
}

void Note::set_led_pin(int pin)
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
  digitalWrite(this->pin, b_note_on ? HIGH : LOW);
}

int Note::get_note_count() 
{
    return note_count;
}


boolean Note::set_state(boolean state)
{
  if (b_note_on != state) {
    b_note_on = state;
    digitalWrite(this->pin, b_note_on ? HIGH : LOW);
    if (b_note_on) {
      Note::note_count++;
    } else {
      Note::note_count--;
    }
  }
  return b_note_on;
}

boolean Note::is_note_on()
{
   return b_note_on; 
}

boolean Note::toggle_state()
{
  return set_state(!b_note_on);
}
