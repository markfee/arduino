#include "mcp4922.h"
#include "envStage.h"
#include "envelope.h"

Mcp4922* pDac;

EnvelopeStage* pCurrentStage;
EnvelopeStage* pAttackStage;
EnvelopeStage* pDecayStage;
EnvelopeStage* pReleaseStage;
Envelope* pEnvelope;

#define TRIGGER_IN 0

void setup() 
{
  pDac = new Mcp4922(10);
// Max Value of unsigned long == 4,294,967,295
  pAttackStage      = new EnvelopeStage(0, 4095, 10000, "attack");
  pDecayStage       = new EnvelopeStage(4095, 0, 3000000, "  decay");
  pReleaseStage     = new EnvelopeStage(4095, 0, 300000, "    release");

  pAttackStage->set_next_stage(pDecayStage);
  pAttackStage->set_release_stage(pReleaseStage);
  pDecayStage->set_release_stage(pReleaseStage);

  pEnvelope = new Envelope(pAttackStage, TRIGGER_IN);

  pAttackStage->set_parent(pEnvelope);
  pAttackStage->set_parent(pEnvelope);
  pDecayStage->set_parent(pEnvelope);

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