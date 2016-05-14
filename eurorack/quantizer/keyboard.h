#ifndef __KEYBOARD__H
#define __KEYBOARD__H
#include "Arduino.h"


class Keyboard {
  private:  
    int current_key;
    int pin;
  public:
    Keyboard(int pin);
    int detect_key_down(); // checks for a new key press and returns the key pressed
};
#endif



