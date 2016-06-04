/*envolope.h*/
#ifndef __ENVELOPE__H
#define __ENVELOPE__H
#include "Arduino.h"

class EnvelopeStage;

class Envelope {
    public:
    enum TRIGGER_STATE {
        ON, OFF
    };
    enum {
        MINIMUM_VALUE = 0, MAXIMUM_VALUE = 4095
    };
    public:
        Envelope(EnvelopeStage* pFirstStage, int trigger_pin);
        void process_trigger(); // Read and process the trigger pin
        long get_value();  // Returns the value at the current time
        EnvelopeStage* get_current_stage();
        void on_end_of_stage();
        inline TRIGGER_STATE get_state() { return state; }
    private:
        EnvelopeStage* pFirstStage;
        EnvelopeStage* pCurrentStage;

        TRIGGER_STATE state;

        void on_trigger_on();
        void on_trigger_off();
        
        int trigger_pin;
};

#endif