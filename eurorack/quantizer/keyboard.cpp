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
    : (c < 110) ? 2
    : (c < 185) ? 4
    : (c < 250) ? 5
    : (c < 300) ? 7
    : (c < 340) ? 9
    : (c < 380) ? 11
    : (c < 400) ? 10
    : (c < 435) ? 8
    : (c < 465) ? 6
    : (c < 490) ? 3
    : (c < 600) ? 1
    : c;
    if (key <= 12 && current_key != key) {
        current_key = key;
//        Serial.println(c);
        return current_key;
    }
//    Serial.println(c);
    return -1;
}
