/*mcp2002.cpp*/

// **************************************************
// SPI for DAC
#include "Arduino.h"
#include "mcp4922.h"


Mcp4922::Mcp4922(int slaveSelect) 
{
    SLAVESELECT0 = slaveSelect;
    byte clr;
    pinMode(DATAOUT, OUTPUT);
    pinMode(SPICLOCK,OUTPUT);
    pinMode(SLAVESELECT0,OUTPUT);
    digitalWrite(SLAVESELECT0,HIGH); //disable device

    SPCR = (1<<SPE)|(1<<MSTR) | (0<<SPR1) | (0<<SPR0);
    clr=SPSR;
    clr=SPDR;
    delay(10);
}

// write out through DAC A
void Mcp4922::write_valueB(int sample)
{
  // splits int sample in to two bytes
  byte dacSPI0 = 0;
  byte dacSPI1 = 0;
  dacSPI0 = (sample >> 8) & 0x00FF; //byte0 = takes bit 15 - 12
  dacSPI0 |= (1 << 7);    // A/B: DACa or DACb - Forces 7th bit  of    x to be 1. all other bits left alone.
  dacSPI0 |= 0x10;
  dacSPI1 = sample & 0x00FF; //byte1 = takes bit 11 - 0
  dacSPI0 |= (1<<5);  // set gain of 1
  digitalWrite(SLAVESELECT0,LOW);
  SPDR = dacSPI0;             // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };

  SPDR = dacSPI1;
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  digitalWrite(SLAVESELECT0,HIGH);
  //delay(2);
}

// write out through DAC B
void Mcp4922::write_valueA(int sample)
{
  // splits int sample in to two bytes
  byte dacSPI0 = 0;
  byte dacSPI1 = 0;
  dacSPI0 = (sample >> 8) & 0x00FF; //byte0 = takes bit 15 - 12
  dacSPI0 |= 0x10;
  
  dacSPI1 = sample & 0x00FF; //byte1 = takes bit 11 - 0
  dacSPI0 |= (1<<5);  // set gain of 1
  
  digitalWrite(SLAVESELECT0,LOW);
  SPDR = dacSPI0;             // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };


  SPDR = dacSPI1;
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  digitalWrite(SLAVESELECT0,HIGH);
  //delay(2);
}