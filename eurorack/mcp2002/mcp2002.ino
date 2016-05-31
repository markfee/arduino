/*
  MCP4922 test
  outputs steadily rising voltages through both DACs
  
  Steve Woodward, 2010
  most code borrowed from
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
  4922 pin 14 DAC A > 1k resistor > synth CV in


1V/O 5v 0v 5v       0v
  14 13 12 11 10 09 08 
   |  |  |  |  |  |  |
   -------------------
   )                 |
   -------------------
   |  |  |  |  |  |  |
  01 02 03 04 05 06 07   
  5v    10 13 11  
 */
 

// MCP4922 DAC out
#define DATAOUT 11//MOSI
#define DATAIN 12//MISO - not used, but part of builtin SPI
#define SPICLOCK  13//sck
#define SLAVESELECT0 10//ss

int i = 0;

void setup() {
  SPI_setup();
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

void loop() {
  write_note(round(68.266666667 * note));
  note+=30*dir;
  if (note >= 60) {
    note = 59;
    dir = -1;
  } else if (note <= 0) {
    note = 0;
    dir = 1;
  }

 delay(200);
 return;  



 i++;
 Serial.println(i);
 write_note(i);
 if(i>=4096) {
  i=0; 
 }
}

void write_note(int i) {
  write_valueY(i);
  write_valueX(i);
}




// **************************************************
// SPI for DAC

void SPI_setup(){

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
void write_valueX(int sample)
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
void write_valueY(int sample)
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