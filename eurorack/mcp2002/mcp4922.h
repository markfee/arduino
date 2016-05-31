/*mcp2002.h*/
#ifndef __MCP4922__H
#define __MCP4922__H
#include "Arduino.h"

/*
  code is based on Steve Woodward's code from 2010
  who himself credited 
  http://mrbook.org/blog/2008/11/22/controlling-a-gakken-sx-150-synth-with-arduino/
  
  connections
  ====================================================
  
  +5v           > 4922 pin 1
  Ard pin 10    > 4922 pin 3   (SS - slave select)
  Ard pin 13    > 4922 pin 4   (SCK - clock)
  Ard pin 11    > 4922 pin 5   (MOSI - data out)
  Ground        > 4922 pin 8   (LDAC)
  +5v           > 4922 pin 11  (voltage ref DAC B)
  Ground        > 4922 pin 12
  +5v           > 4922 pin 13  (voltage ref DAC A)
  4922                 pin 14   DAC A > 1k resistor > synth CV in


  vA 5v 0v 5v vB    0v
  14 13 12 11 10 09 08 
   |  |  |  |  |  |  |
   -------------------
   )                 |
   -------------------
   |  |  |  |  |  |  |
  01 02 03 04 05 06 07   
  5v    10 13 11  
 */

class Mcp4922 {
  private:  
    int SLAVESELECT0;  //ss Slave Select (can be used to select different DACs on different pins)
    const int DATAOUT       =   11;
//    const int DATAIN      =   12; //MISO - not used, but part of builtin SPI
    const int SPICLOCK      =   13; //sck
  public:
    Mcp4922(int slaveSelect);
    void write_valueA(int sample);
    void write_valueB(int sample);
};
#endif
