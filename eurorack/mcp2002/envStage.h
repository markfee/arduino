/*envStage.h*/
#ifndef __ENV_STAGE__H
#define __ENV_STAGE__H
#include "Arduino.h"

/*
    Represents a linear stage of an envelope that rises or drops from startValue to endValue over n micro seconds
*/
class EnvelopeStage {
  private:  
    unsigned int micro_length; // stage length in microseconds. 1*10^(-6) Seconds
    int startValue;
    int endValue;
    int direction; // 1 or -1
    unsigned long start_time;
    bool current_state; // = on (true) or off (false)
    void trigger_on();  // Start the envelope
    void trigger_off(); // Stop the envelope
  public:
    EnvelopeStage(int startValue, int endValue, unsigned int length );
    void set_state(bool state); // Start the envelope
    int get_value();  // Returns the value at the current time
};
#endif
