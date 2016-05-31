#include "mcp4922.h"
#include "envStage.h"


int i = 0;

Mcp4922* pDac;
EnvelopeStage* pAttackStage;

void setup() {
  pDac = new Mcp4922(10);
  pAttackStage = new EnvelopeStage(0, 2000, 6000000);
//  SPI_setup();
  Serial.begin(9600);
}
// 4
// 840  == 1 Octave 1 Volt
// 1661 == 2.0
// 2485 == 3.0
// 3310 == 4.00
// 3722 == 4.5

int note = 0;
int dir = 1;
bool bA = true;
int count = 0;
void loop() {

  if (count % 10000) {
    delay(100);
    pAttackStage->set_state(true);
  }

  count++;
  pDac->write_valueB(pAttackStage->get_value());

  return;

  if (bA) {
      pDac->write_valueA(round(68.266666667 * note));
  } else {
      pDac->write_valueB(round(68.266666667 * note));
  }
  note+=30*dir;
  if (note >= 60) {
    note = 59;
    dir = -1;
  } else if (note <= 0) {
    note = 0;
    dir = 1;
    bA = ! bA;
  }

 delay(100);
 return;  
}