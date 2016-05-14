#ifndef __NOTE__H
#define __NOTE__H
#include "Arduino.h"


class Note {
  private:
    boolean b_note_on;
    int pin;
  public:
    Note();
    void set_led_pin(int pin);
    boolean toggle_state();
    boolean is_note_on();
};
#endif
