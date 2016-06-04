#include "mcp4922.h"
#include "envStage.h"
#include "envelope.h"

Mcp4922* pDac;

EnvelopeStage* pCurrentStage;
EnvelopeStage* pAttackStage;
EnvelopeStage* pDecayStage;
EnvelopeStage* pSustainStage;
//EnvelopeStage* pSustainReleaseStage;
EnvelopeStage* pReleaseStage;
Envelope* pEnvelope;

#define TRIGGER_IN 0
#define ATTACK_IN 1
#define DECAY_IN 2
#define SUSTAIN_IN 3

void setup() 
{
  pDac = new Mcp4922(10);
// Max Value of unsigned long == 4294967295

/* Variables ==
  rate 0.5 - 5
  attack time, 
  decay time, 
//  sustain time (can be infinite?), 
  sustain level, 
  release time
 also make exponential*/

  pAttackStage          = new EnvelopeStage(0, 4095,      100000, "attack");
  pDecayStage           = new EnvelopeStage(4095, 3000,  3000000, "  decay");

  pSustainStage         = new EnvelopeStage(3000, 0,   0, "   sustain");
//  pSustainReleaseStage  = new EnvelopeStage(3000, 0,    5000000, "    releaseSus");
  pReleaseStage         = new EnvelopeStage(4095, 0,    1000000, "    release");

  pAttackStage->set_next_stage(pDecayStage);
  pAttackStage->set_release_stage(pReleaseStage);
  
  pDecayStage->set_next_stage(pSustainStage);
  pDecayStage->set_release_stage(pReleaseStage);
  
  pSustainStage->set_release_stage(pReleaseStage);


  pEnvelope = new Envelope(pAttackStage, TRIGGER_IN);

  pAttackStage->set_parent(pEnvelope);
  pAttackStage->set_parent(pEnvelope);
  pDecayStage->set_parent(pEnvelope);
  pSustainStage->set_parent(pEnvelope);

  pCurrentStage = pAttackStage;
  Serial.begin(9600);
}


int count = 0;

void loop() 
{
  pEnvelope->process_trigger();
  pDac->write_valueA(pEnvelope->get_value());
  count++;
  if (Envelope::ON == pEnvelope->get_state() || (count % 10000) == 0) {
    int attack  = analogRead(ATTACK_IN);
    int decay   = analogRead(DECAY_IN);
    int sustain   = analogRead(SUSTAIN_IN);

    pAttackStage->set_length(attack, 5); // from 0 to 1023
    pDecayStage->set_length(decay, 3); // from 0 to 1023
    pReleaseStage->set_length(decay, 0.5); // from 0 to 1023

   Serial.print("attack: "); Serial.println(attack);    
   Serial.print("decay:  "); Serial.println(decay);  
   Serial.print("sustain:  "); Serial.println(sustain);    


    pDecayStage->set_end(sustain, 1);
    pSustainStage->set_start(sustain, 1);
  }
  return;
}