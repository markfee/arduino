/*envStage.cpp*/
#include "envStage.h"

EnvelopeStage::EnvelopeStage(int startValue, int endValue, unsigned int length)
{
    micro_length    =   length;
    startValue      =   startValue;
    endValue        =   endValue;
    direction       =   startValue < endValue ? 1 : -1; // 1 or -1
    start_time      =   0;
    current_state   =   false; // = on (true) or off (false)
}

void EnvelopeStage::set_state(bool state)
{
    if (current_state == state) {
        return; // Do Nothing no change of state
    }

    if (state) { // Triggering ON
        trigger_on();
    } else {
        trigger_off();
    }
}

void EnvelopeStage::trigger_on()
{
    current_state   =   true;
    start_time      =   micros();
}

void EnvelopeStage::trigger_off()
{
    current_state   =   false;
    start_time      =   0;
}

int  EnvelopeStage::get_value()
{
    if (!current_state) {
        return 0;
    }

    unsigned long time_since_trigger = micros() - start_time;
    if (time_since_trigger > micro_length) 
    {
        trigger_off();
        return endValue;
    }

    return (time_since_trigger * endValue) / micro_length;
}
