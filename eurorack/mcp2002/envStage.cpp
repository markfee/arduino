/*envStage.cpp*/
#include "envStage.h"

EnvelopeStage::EnvelopeStage(long startValue, long endValue, unsigned long length, char* pName)
{
    this->micro_length    =   length;
    this->startValue      =   startValue;
    this->endValue        =   endValue;
    this->start_time      =   0;
    this->current_state   =   false; // = on (true) or off (false)
    this->note_on         =   false;

    this->pCurrentStage     =   this;
    this->pNextStage        =   NULL;
    this->pReleaseStage     =   NULL;
    this->pName             =   pName;
    latestValue             =   0;
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

EnvelopeStage* EnvelopeStage::get_current_stage()
{
    if (this->pCurrentStage == this) {
        return this;
    } 

    if (this->pCurrentStage != NULL) {
        this->pCurrentStage = this->pCurrentStage->get_current_stage();
    }

    if (this->pCurrentStage == NULL) {
        this->pCurrentStage = this;
    }
    return this->pCurrentStage;
}

void EnvelopeStage::set_next_stage(EnvelopeStage* pStage)
{
    this->pNextStage        =   pStage;
}

void EnvelopeStage::set_release_stage(EnvelopeStage* pStage)
{
    this->pReleaseStage     =   pStage;
}


void EnvelopeStage::trigger_on()
{
    Serial.print(this->pName); Serial.println(" trigger on");
    this->pCurrentStage = this;
    current_state   =   true;
    note_on         =   true;
    start_time      =   micros();
    if (this->pNextStage) {
        this->pNextStage->current_state = this->pNextStage->note_on = false;
    }
    if (this->pReleaseStage) {
        this->pReleaseStage->current_state = this->pReleaseStage->note_on = false;
    }
}

void EnvelopeStage::trigger_off()
{
    current_state   =   false;
    note_on         =   false;

    start_time      =   0;

    // If we are currently on the next stage, then let it know that a key has been released
    if (this->pNextStage != NULL && this->pNextStage == this->pCurrentStage) {
        this->pCurrentStage->trigger_off();
        this->pCurrentStage = this;
    } else if (this->latestValue > 0 && this->pReleaseStage != NULL)
    { // or lets kick in the release stage
        this->pCurrentStage = this->pReleaseStage;
        this->pReleaseStage->startValue = latestValue;
        this->pReleaseStage->set_state(false); // Trick the release stage into thinking a key is down
        this->pReleaseStage->set_state(true);
    } else 
    { // if there is no next stage and no current stage then set current stage back to this.
        this->pCurrentStage = this;
    }
    Serial.print(this->pName); Serial.println(" trigger off");
}

void EnvelopeStage::end_of_stage()
{
    Serial.print(this->pName); Serial.println(" end of stage");
    current_state   =   false;
    start_time      =   0;

    if (this->pNextStage)
    {
        this->pCurrentStage = this->pNextStage;
        this->pNextStage->trigger_on();
    } else {
        // If there is no next stage then treat as if the gate has been released
        latestValue = 0;
        trigger_off();
    }
}

long  EnvelopeStage::get_value()
{
    if (!note_on) {
        latestValue = 0;
    } else if (!current_state) {
        latestValue = endValue;
    } else {
        unsigned long time_since_trigger = micros() - start_time;

        if (time_since_trigger > micro_length) 
        {
            end_of_stage();
            latestValue = endValue;
        } else {
            if (endValue > startValue) {
                latestValue =  min(((time_since_trigger * (endValue - startValue)) / micro_length), max(startValue, endValue));    
            } else {
                latestValue =  startValue - min(((time_since_trigger * (startValue - endValue)) / micro_length), max(startValue, endValue));
            }
        }
    }
    return latestValue;
}
