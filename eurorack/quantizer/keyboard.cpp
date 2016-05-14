#include "keyboard.h"


Keyboard::Keyboard(int pin)
{
  this->pin = pin;
  current_key = 0;
  pinMode(pin, INPUT_PULLUP); // sets analog pin for input 
}

int Keyboard::detect_key_down()
// returns the button number pressed, or zero for none pressed 
// int pin is the analog pin number to read 
{
  int c = analogRead(pin); // get the analog value  
  int key = (c>1000) ? -1
    : (c < 25) ? 0
    : (c < 110) ? 1
    : (c < 185) ? 2
    : c;
    if (key <= 12 && current_key != key) {
        current_key = key;
        Serial.println(c);
        return current_key;
    }
    return -1;
}
