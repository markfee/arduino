#include "Note.h";
#include "keyboard.h";

const int CV_IN = A0;
const int CV_OUT = A1;
const int KEYBOARD_IN = A5;
const boolean DEBUG = true;
const int OCTAVE_RANGE = 5;

const int MIN_NOTE = 0;
const int MAX_NOTE = 60;

// MCP4922 DAC out
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
  
 */
 
/*
#define DATAOUT 11//MOSI
#define DATAIN 12//MISO - not used, but part of builtin SPI
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss
*/
/*11*/ #define DATAOUT        11 // (MCP4922 PIN 5 SDI)
/*12*/ #define DATAIN         12  // MISO - not used, but part of builtin SPI
/*13*/ #define SPICLOCK       13 // (MCP4922 PIN 4 SCK)
/*10*/ #define SLAVESELECT0   A4 // (MCP4922 PIN 3 CS)

Note notes[12];
Keyboard* keyboard;
int notes_on_count = 12;
/*                          C # D # E F # G # A # B */
char*   NOTE_NAMES[12]  = {"C ","C#","D ","D#","E ","F ","F#","G","F#","A ","A#","B "};
boolean MAJOR_SCALE[12]  = {1,0,1,0,1,1,0,1,0,1,0,1};
boolean MINOR_SCALE[12]  = {1,0,1,1,0,1,0,1,0,1,0,1};
boolean DORIAN_SCALE[12] = {1,0,1,1,0,1,0,1,0,1,1,0};

int black_notes[5] = {1,3,6,8,10};
int white_notes[7] = {0,2,4,5,7,9,11};

void set_scale(boolean scale[], int size = 12) 
{
  for(int i = 0; i < size; i++) 
  {
    notes[i].set_state(scale[i]);
  }
}

void print_notes()
{
/*
 |X|X|_|X|X|X|   
| | | | | | | | | |
*/
char* chBlack = "_|X|X|_|X|X|X|_";  //[15]
char* chWhite = "|X|X|X|X|X|X|X|";  //[15]
// PRINT THE BLACK NOTES
  int note_index = 0;
  for (int i= 0; i < 15; i++) {
    if (    chBlack[i] == 'X') {
        if(!notes[black_notes[note_index]].is_note_on()) {
          Serial.print(" ");
        } else {
          Serial.print(chBlack[i]);
        }
        note_index++;
    } else {
      Serial.print(chBlack[i]);
    }
  }
  Serial.println("");  
// PRINT THE WHITE NOTES
  note_index = 0;
  for (int i= 0; i < 15; i++) {
    if (    chWhite[i] == 'X' ) {
        if ( !notes[white_notes[note_index]].is_note_on()) {
          Serial.print(" ");
      } else {
        Serial.print(chWhite[i]);
      }
      note_index++;
    } else {
      Serial.print(chWhite[i]);
    }
  }
  Serial.println("");
}


void setup()
{
  pinMode(CV_OUT, OUTPUT);

  if (DEBUG) {
    Serial.begin(9600);          //  setup serial
  }
//  UCSR0B = 0;
  keyboard = new Keyboard(KEYBOARD_IN);
  
  for(int i = 0; i < 12; i++) {
    notes[i].set_led_pin(13 - i);
  }
  
  set_scale(MAJOR_SCALE);
  SPI_setup();
  
}

int get_closest_note_offset(int note, int noteIndex, boolean debug = false) 
{
  if (notes[noteIndex].is_note_on()) {
    if (debug) {
      Serial.println("Note is ON!");
    }
    return note;
  }
  for (int i = 1; i < 12; i++)
  {

    if (note + i < MAX_NOTE) { // If the next note up is in range
      int higher_index = noteIndex + i;
      if (higher_index >= 12) { // drop an octave if you've passed twelve (the returned note will still be higher)
        higher_index -= 12;
      }
      if (notes[higher_index].is_note_on()) {
        if (debug) {
          Serial.print("Returning HIGHER NOTE "); Serial.println(note + i);
        }
        return note + i;
      }
    }

    if (note - i >= MIN_NOTE) { // If the next note up is in range
      int lower_index = noteIndex - i;
      if (lower_index < 0) { // increase an octave if you've passed zero (the returned note will still be lower)
        lower_index += 12;
      }
      if (notes[lower_index].is_note_on()) {
        if (debug) {
          Serial.print("Returning LOWER NOTE "); Serial.println(note - i);
        }
        return note - i;
      }
    }
  }
  return -1;
}

int loopCount = 0;
int i = 0;

void loop()
{
//  i++;
//  write_note(i);
//  if(i>=4096) {
//    i=0; 
//  }

//  delay(500);
//  return;

 int key = -1;
  // Onlyt Read the keyboard one in 1000 cycles to free up the processor to process CV
  if (0 == (loopCount % 1000)) {
    key = keyboard->detect_key_down();
    if (key > -1) {
      notes_on_count += (notes[key].toggle_state() ? 1: -1);
    }  
  }
  
  long cv_in_value                = analogRead(CV_IN);
  if (cv_in_value > 0 && cv_in_value < 275) {
    // OFFSET the low values to compensate for digital errors with low values
    cv_in_value +=5;
  }
  long octave_in_value            =  cv_in_value *  5 / 1024;
  long closest_note_index         = (cv_in_value * 60 / 1024) % 12;
  long closest_note_number        = closest_note_index + 12 * octave_in_value;
  long closest_note_number2       = cv_in_value * 60 / 1024;
  long closest_note_on_number     = get_closest_note_offset(closest_note_number, closest_note_index);
  long octave_out_value           = closest_note_on_number / 60;
  long closest_note_on_index      = closest_note_on_number % 12;
//  bits_per_note = 1024 / 60
  long cv_out_value               = closest_note_on_number * 4096 / 60;
//  cv_out_value = 4095;
  analogWrite(CV_OUT, (loopCount % 1024));
  analogWrite(CV_OUT, cv_out_value);
  write_note(cv_out_value);
  
//  analogWrite(CV_OUT, 250);

  if (loopCount >= 10000 || key > -1) {
    loopCount = 0;
    if (DEBUG) {
      closest_note_on_number = get_closest_note_offset(closest_note_number, closest_note_index, DEBUG);
      Serial.print("cv_in_value:            ");   Serial.println(cv_in_value);
      Serial.print("cv_out_value:           ");   Serial.println(cv_out_value);

      Serial.print("octave_in_value:        ");   Serial.println(octave_in_value);
      Serial.print("closest_note_index:     ");   Serial.print(NOTE_NAMES[closest_note_index]);        Serial.println(octave_in_value);
      Serial.print("closest_note_on_index:  ");   Serial.print(NOTE_NAMES[closest_note_on_index]);     Serial.println(octave_out_value);
      
      Serial.print("closest_note_number:    ");   Serial.println(closest_note_number);
      Serial.print("closest_note_number2:   ");   Serial.println(closest_note_number2);
      Serial.print("closest_note_on_number: ");   Serial.println(closest_note_on_number);
      
      Serial.print("key pressed:            ");   Serial.println(key);
      Serial.print("notes_on_count:         ");   Serial.println(notes_on_count);
      Serial.print("EXPECTED V OUT:         ");   Serial.println(0.083333333 * closest_note_on_number);
      print_notes();
    }
  } else {
    loopCount++;
  }
}

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


void write_note(int i) {
//  return;
  write_valueY(i);
  write_valueX(i);
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
  SPDR = dacSPI0;			  // Start the transmission
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
  SPDR = dacSPI0;			  // Start the transmission
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

