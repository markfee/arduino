/*envStage.h*/
#ifndef __ENV_STAGE__H
#define __ENV_STAGE__H
#include "Arduino.h"

/*
    Represents a linear stage of an envelope that rises or drops from startValue to endValue over n micro seconds
*/
class EnvelopeStage {
  private:  
    long startValue;
    long endValue;
//    int direction; // 1 or -1
    unsigned long start_time;
    unsigned long micro_length; // stage length in microseconds. 1*10^(-6) Seconds
    bool current_state; // = on (true) or off (false)
    bool note_on; // = on (true) or off (false)
    void trigger_on();  // A trigger on is recieved
    void trigger_off(); // A trigger off is recieved
    void end_of_stage(); // The stage reaches it's end

    EnvelopeStage* pCurrentStage;
    EnvelopeStage* pNextStage;
    EnvelopeStage* pReleaseStage;
    char* pName;
    long latestValue;
  public:
    EnvelopeStage(long startValue, long endValue, unsigned long length, char* pName);
    void set_state(bool state); // Start the envelope
    long get_value();  // Returns the value at the current time

    void set_next_stage(EnvelopeStage*);        // Sets the stage to trigger when this stage finishes
    void set_release_stage(EnvelopeStage*);     // Sets the stage to trigger when the note is released.
    EnvelopeStage* get_current_stage();
};
#endif
