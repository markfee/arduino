#include "Note.h"

Note::Note()
{
  this->pin = pin;
  b_note_on = false;
}

void Note::set_led_pin(int pin)
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
  digitalWrite(this->pin, LOW);
}

boolean Note::toggle_state()
{
  b_note_on = ! b_note_on;
  digitalWrite(this->pin, b_note_on ? HIGH : LOW);
}

boolean Note::is_note_on()
{
   return b_note_on; 
}

