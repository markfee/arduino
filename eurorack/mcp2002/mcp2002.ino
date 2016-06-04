#include "mcp4922.h"
#include "envStage.h"
#include "envelope.h"

Mcp4922* pDac;

EnvelopeStage* pCurrentStage;
EnvelopeStage* pAttackStage;
EnvelopeStage* pDecayStage;
EnvelopeStage* pSustainStage;
EnvelopeStage* pSustainReleaseStage;
EnvelopeStage* pReleaseStage;
Envelope* pEnvelope;

#define TRIGGER_IN 0

void setup() 
{
  pDac = new Mcp4922(10);
// Max Value of unsigned long == 4294967295
  pAttackStage          = new EnvelopeStage(0, 4095,      10000, "attack");
  pDecayStage           = new EnvelopeStage(4095, 3000, 3000000, "  decay");

  pSustainStage         = new EnvelopeStage(3000, 0,   20000000, "   sustain");
  pSustainReleaseStage  = new EnvelopeStage(3000, 0,    5000000, "    releaseSus");
  pReleaseStage         = new EnvelopeStage(4095, 0,    1000000, "    release");

  pAttackStage->set_next_stage(pDecayStage);
  pAttackStage->set_release_stage(pReleaseStage);
  
  pDecayStage->set_next_stage(pSustainStage);
  pDecayStage->set_release_stage(pReleaseStage);
  
  pSustainStage->set_release_stage(pSustainReleaseStage);


  pEnvelope = new Envelope(pAttackStage, TRIGGER_IN);
  pAttackStage->set_parent(pEnvelope);
  pAttackStage->set_parent(pEnvelope);
  pDecayStage->set_parent(pEnvelope);
  pSustainStage->set_parent(pEnvelope);
  pSustainReleaseStage->set_parent(pEnvelope);

  pCurrentStage = pAttackStage;
  Serial.begin(9600);
}


int count = 0;

void loop() 
{
  pEnvelope->process_trigger();
  pDac->write_valueA(pEnvelope->get_value());
  return;

  int val = analogRead(TRIGGER_IN);    // read the input pin

  pAttackStage->set_state((val > 100));

  int nVal2 = pAttackStage->get_current_stage()->get_value();

  pDac->write_valueA(nVal2);
}