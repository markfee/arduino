#include "mcp4922.h"
#include "envStage.h"

Mcp4922* pDac;

EnvelopeStage* pCurrentStage;
EnvelopeStage* pAttackStage;
EnvelopeStage* pDecayStage;
EnvelopeStage* pReleaseStage;

#define TRIGGER_IN 0

void setup() 
{
  pDac = new Mcp4922(10);

  pAttackStage      = new EnvelopeStage(0, 4095, 100000, "attack");
  pDecayStage       = new EnvelopeStage(4095, 0, 1000000, "decay");
  pReleaseStage     = new EnvelopeStage(4095, 0, 1000000, "release");

  pAttackStage->set_next_stage(pDecayStage);
  pAttackStage->set_release_stage(pReleaseStage);
  pDecayStage->set_release_stage(pReleaseStage);

  pCurrentStage = pAttackStage;

  Serial.begin(9600);
}


int count = 0;

void loop() 
{
  int val = analogRead(TRIGGER_IN);    // read the input pin

  pAttackStage->set_state((val > 100));

    int nVal2 = pAttackStage->get_current_stage()->get_value();

  pDac->write_valueA(nVal2);
}