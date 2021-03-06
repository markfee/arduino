/*envStage.cpp*/
#include "envStage.h"
#include "envelope.h"

EnvelopeStage::EnvelopeStage(long startValue, long endValue, unsigned long length, char* pName)
{
    this->micro_length    =   length;
    this->startValue      =   startValue;
    this->endValue        =   endValue;
    this->valueRange      =   abs(endValue - startValue);
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
//    Serial.print(this->pName); Serial.println(" starting");    
    this->startValue = initial_value;
    this->start_time      =   micros();
}

int EnvelopeStage::stop()
{
//    Serial.print(this->pName); Serial.println(" stopping");    
    this->start_time = 0;
    return this->latestValue;
}

long EnvelopeStage::get_value()
{
    if (micro_length == 0) { // Implies infinite
        return latestValue = startValue;
    }

    unsigned long time_since_trigger = micros() - start_time;

    if (time_since_trigger > micro_length) 
    {
        this->pParent->on_end_of_stage();
        latestValue = endValue;
    } else {

        double percentComplete  = 1.0 * time_since_trigger / micro_length;
        double multiplier       = pow(percentComplete, 0.5);

        if (endValue > startValue) {
            latestValue =  startValue + min(multiplier * valueRange, endValue);
        } else {
            latestValue =  startValue - min(multiplier * valueRange, startValue);
        }
    }
    return latestValue;
}

long  EnvelopeStage::convert_read_value(int val, float curve, unsigned long max_value)
{
    return pow((1.0 * val / 1024), curve) * max_value;
}

void EnvelopeStage::set_length(int length, float curve) // from 0 to 1023
{
//    unsigned long new_length = pow((1.0 * length / 1024), curve) * MAX_LENGTH;
//    this->micro_length = max(1, new_length);

    this->micro_length = max(1, convert_read_value(length, curve, MAX_LENGTH));
}

void EnvelopeStage::set_start(int val, float curve)
{
    this->startValue = convert_read_value(val, curve, Envelope::MAXIMUM_VALUE);
}

void EnvelopeStage::set_end(int val, float curve)
{
    this->endValue = convert_read_value(val, curve, Envelope::MAXIMUM_VALUE);
}


