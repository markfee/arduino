#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal.h>
int RECV_PIN = 13;


IRrecv irrecv(RECV_PIN);
decode_results results;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
char* strMessage = "                Hello Sydney and Albert         ";
char* strClear   = "                                ";
int timer;

void setup() {
  irrecv.enableIRIn(); // Start the receiver
// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(strClear);
  Serial.begin(9600);  
}

char buffer[64];



void loop() {
  int seconds = millis();
  if (irrecv.decode(&results)) {
    if (results.value != 0xFFFFFFFF) {
      parseRemote(results.value, buffer);
      Serial.print(buffer);  
      Serial.println(results.value, HEX);  
      lcd.setCursor(0, 0);  
      lcd.print(buffer);
      lcd.setCursor(0, 1);  
      lcd.print(results.value, HEX);
      timer = seconds;   
    }
    irrecv.resume(); // Receive the next value
  }
  
  if (seconds - timer > 4000) {
    lcd.setCursor(0, 0);  
    lcd.print(strClear);
  }
  int pos = seconds % 40;
  int row = pos / 16;
}

char* parseRemote(unsigned long value, char* chbuffer) {
  char keyPressed = 0;
  switch (value) {
    case 0xC101E57B:    case 0xFF6897:      keyPressed = '0'; break;
    case 0x9716BE3F:    case 0xFF30CF:      keyPressed = '1'; break;
    case 0x3D9AE3F7:    case 0xFF18E7:      keyPressed = '2'; break;
    case 0x6182021B:    case 0xFF7A85:      keyPressed = '3'; break;
    case 0x8C22657B:    case 0xFF10EF:      keyPressed = '4'; break;
    case 0x488F3CBB:    case 0xFF38C7:      keyPressed = '5'; break;
    case 0x449E79F:     case 0xFF5AA5:      keyPressed = '6'; break;
    case 0x32C6FDF7:    case 0xFF42BD:      keyPressed = '7'; break;
    case 0x1BC0157B:    case 0xFF4AB5:      keyPressed = '8'; break;
    case 0x3EC3FC1B:    case 0xFF52AD:      keyPressed = '9'; break;
  }
  if (keyPressed == 0) {
    word right_byte = value & 0xFFFF;
    word left_byte = ( value >> 16 ) & 0xFFFF;
    sprintf(chbuffer, "Unmapped Key: 0x%04X%04X   ", left_byte, right_byte);
  } else {
    sprintf(chbuffer, "Key: %c  ", keyPressed);
  }
  return chbuffer;
}
