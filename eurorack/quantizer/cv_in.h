#ifndef __CV_IN__H
#define __CV_IN__H
#include "Arduino.h"


class CV_In {
  private:  
    int current_key;
    int pin;
  public:
    CV_In(int pin);
    int detect_key_down(); // checks for a new key press and returns the key pressed
};
#endif



