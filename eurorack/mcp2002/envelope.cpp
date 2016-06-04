/*Envelope.cpp*/
#include "envelope.h"
#include "envStage.h"

Envelope::Envelope(EnvelopeStage* pFirstStage, int trigger_pin)
{
    this->pFirstStage       =   pFirstStage;
    this->trigger_pin       =   trigger_pin;
    this->pCurrentStage     =   NULL;
    this->state             =   Envelope::OFF;
}

void Envelope::process_trigger()
{
    TRIGGER_STATE new_state = (analogRead(trigger_pin) > 100 ? ON : OFF);
    if (new_state != this->state)
    {
        this->state = new_state;
        if (new_state == ON) {
            on_trigger_on();
        } else {
            on_trigger_off();
        }

    }
}

long Envelope::get_value()
{
    if (this->pCurrentStage) {
        return this->pCurrentStage->get_value();
    }
    return 0;
}

void Envelope::on_trigger_on()
{
    if (this->pCurrentStage) {
        this->pCurrentStage->stop();
    }
    this->pCurrentStage = this->pFirstStage;
    this->pCurrentStage->start();
}

void Envelope::on_end_of_stage()
{
    if (this->pCurrentStage) {
        this->pCurrentStage->stop();
        if (this->pCurrentStage = this->pCurrentStage->get_next_stage())
        {
            this->pCurrentStage->start();
        }
    }
}

void Envelope::on_trigger_off()
{
    Serial.println("on_trigger_off()");
    if ( this->pCurrentStage ) {
        int final_value = this->pCurrentStage->stop(); // Stop the current stage
        Serial.println(final_value);
        if (this->pCurrentStage = this->pCurrentStage->get_release_stage()) {
            Serial.println("starting release stage");
            this->pCurrentStage->start(final_value); // and start the release stage (if the current stage has one)
        }
    }
}
