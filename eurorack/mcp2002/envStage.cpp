/*envStage.cpp*/
#include "envStage.h"
#include "envelope.h"

EnvelopeStage::EnvelopeStage(long startValue, long endValue, unsigned long length, char* pName)
{
    this->micro_length    =   length;
    this->startValue      =   startValue;
    this->endValue        =   endValue;
    this->start_time      =   0;
    this->current_state   =   false; // = on (true) or off (false)
    this->note_on         =   false;

//    this->pCurrentStage     =   this;
    this->pNextStage        =   NULL;
    this->pReleaseStage     =   NULL;
    this->pName             =   pName;
    latestValue             =   0;
    Serial.print(this->pName); Serial.print(" length - "); Serial.println(micro_length);
}

void EnvelopeStage::set_state(bool state)
{
    if (note_on == state) {
        return; // Do Nothing no change of state
    }

    if (state) { // Triggering ON
        trigger_on();
    } else {
        trigger_off();
    }
}

void EnvelopeStage::set_next_stage(EnvelopeStage* pStage)
{
    this->pNextStage        =   pStage;
}

void EnvelopeStage::set_release_stage(EnvelopeStage* pStage)
{
    this->pReleaseStage     =   pStage;
}

void EnvelopeStage::start()
{
    this->start(this->startValue);
}

void EnvelopeStage::start(int initial_value)
{
    Serial.print(this->pName); Serial.println(" starting");    
    this->startValue = initial_value;
    this->start_time      =   micros();
}

int EnvelopeStage::stop()
{
    Serial.print(this->pName); Serial.println(" stopping");    
    this->start_time = 0;
    return this->latestValue;
}


long  EnvelopeStage::get_value()
{
    unsigned long time_since_trigger = micros() - start_time;
    if (time_since_trigger > micro_length) 
    {
        this->pParent->on_end_of_stage();
        latestValue = endValue;
    } else {
        if (endValue > startValue) {
            latestValue =  startValue + min((((1.0 * time_since_trigger/ micro_length) * (endValue - startValue))), endValue);    
        } else {
            latestValue =  startValue - min((((1.0 * time_since_trigger/ micro_length) * (startValue - endValue))), startValue);
        }
    }
    return latestValue;
}
