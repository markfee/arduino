#ifndef __NOTE__H
#define __NOTE__H
#include "Arduino.h"

class Note {
  private:
    boolean b_note_on;
    int pin;
  private: 
    static int note_count;
  public:
    Note();
    static int get_note_count();
    void set_led_pin(int pin);
    boolean set_state(boolean state);
    boolean toggle_state();
    boolean is_note_on();
};



#endif
