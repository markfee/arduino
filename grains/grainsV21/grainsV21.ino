// Grains V2.1
// smooth CV pitch
// 02-06-2015 finetune for PCB V2 added on pin A3
//
// based on the Auduino code by Peter Knight, revised and finetuned by Ginkosynthese for use with cv inputs.
//
// inputs
// Analog in 0: Grain repetition frequency
// Analog in 1: Grain 1 pitch
// Analog in 2: Grain 2 pitch
// output
// Digital 11: Audio out (PWM)


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

uint16_t syncPhaseAcc;
uint16_t syncPhaseInc;
uint16_t grainPhaseAcc;
uint16_t grainPhaseInc;
uint16_t grainAmp;
uint8_t grainDecay;
uint16_t grain2PhaseAcc;
uint16_t grain2PhaseInc;
uint16_t grain2Amp;
uint8_t grain2Decay;

// Map Analogue channels
#define SYNC_CONTROL         (3)
#define SYNC_OFFSET         (0)
#define GRAIN_FREQ_CONTROL   (1)
#define GRAIN2_FREQ_CONTROL  (2)

// Changing these will also requires rewriting audioOn()
//    Output is on pin 11
#define PWM_PIN       11
#define PWM_VALUE     OCR2A
#define LED_PIN       13
#define LED_PORT      PORTB
#define LED_BIT       5
#define PWM_INTERRUPT TIMER2_OVF_vect

const uint16_t freqTable[] PROGMEM = {
69,69,69,69,70,70,70,70,70,71,71,71,71,72,72,72,72,73,73,73,73,
74,74,74,74,75,75,75,75,76,76,76,76,77,77,77,77,78,78,78,78,79,
79,79,79,80,80,80,81,81,81,81,82,82,82,82,83,83,83,84,84,84,84,
85,85,85,86,86,86,86,87,87,87,88,88,88,88,89,89,89,90,90,90,91,
91,91,91,92,92,92,93,93,93,94,94,94,95,95,95,96,96,96,97,97,97,
97,98,98,98,99,99,99,100,100,100,101,101,101,102,102,102,103,103,104,104,104,
105,105,105,106,106,106,107,107,107,108,108,109,109,109,110,110,110,111,111,111,112,
112,113,113,113,114,114,114,115,115,116,116,116,117,117,118,118,118,119,119,120,120,
120,121,121,122,122,122,123,123,124,124,124,125,125,126,126,127,127,127,128,128,129,
129,130,130,130,131,131,132,132,133,133,134,134,134,135,135,136,136,137,137,138,138,
139,139,139,140,140,141,141,142,142,143,143,144,144,145,145,146,146,147,147,148,148,
149,149,150,150,151,151,152,152,153,153,154,154,155,155,156,156,157,157,158,158,159,
159,160,161,161,162,162,163,163,164,164,165,165,166,167,167,168,168,169,169,170,171,
171,172,172,173,173,174,175,175,176,176,177,178,178,179,179,180,181,181,182,182,183,
184,184,185,185,186,187,187,188,189,189,190,190,191,192,192,193,194,194,195,196,196,
197,198,198,199,200,200,201,202,202,203,204,204,205,206,206,207,208,208,209,210,211,
211,212,213,213,214,215,216,216,217,218,218,219,220,221,221,222,223,224,224,225,226,
227,227,228,229,230,231,231,232,233,234,234,235,236,237,238,238,239,240,241,242,242,
243,244,245,246,246,247,248,249,250,251,251,252,253,254,255,256,257,257,258,259,260,
261,262,263,264,264,265,266,267,268,269,270,271,272,273,273,274,275,276,277,278,279,
280,281,282,283,284,285,286,287,288,288,289,290,291,292,293,294,295,296,297,298,299,
300,301,302,303,304,305,306,307,308,310,311,312,313,314,315,316,317,318,319,320,321,
322,323,324,325,327,328,329,330,331,332,333,334,335,337,338,339,340,341,342,343,345,
346,347,348,349,350,352,353,354,355,356,357,359,360,361,362,363,365,366,367,368,370,
371,372,373,375,376,377,378,380,381,382,383,385,386,387,389,390,391,393,394,395,397,
398,399,401,402,403,405,406,407,409,410,411,413,414,416,417,418,420,421,423,424,425,
427,428,430,431,433,434,436,437,438,440,441,443,444,446,447,449,450,452,453,455,456,
458,460,461,463,464,466,467,469,470,472,474,475,477,478,480,482,483,485,486,488,490,
491,493,495,496,498,500,501,503,505,506,508,510,511,513,515,517,518,520,522,524,525,
527,529,531,532,534,536,538,540,541,543,545,547,549,551,552,554,556,558,560,562,564,
566,567,569,571,573,575,577,579,581,583,585,587,589,591,593,595,597,599,601,603,605,
607,609,611,613,615,617,619,621,623,625,627,630,632,634,636,638,640,642,644,647,649,
651,653,655,658,660,662,664,666,669,671,673,675,678,680,682,685,687,689,691,694,696,
698,701,703,705,708,710,713,715,717,720,722,725,727,729,732,734,737,739,742,744,747,
749,752,754,757,759,762,764,767,770,772,775,777,780,783,785,788,791,793,796,799,801,
804,807,809,812,815,817,820,823,826,828,831,834,837,840,842,845,848,851,854,857,860,
862,865,868,871,874,877,880,883,886,889,892,895,898,901,904,907,910,913,916,919,922,
925,928,932,935,938,941,944,947,950,954,957,960,963,966,970,973,976,979,983,986,989,
993,996,999,1003,1006,1009,1013,1016,1020,1023,1027,1030,1033,1037,1040,1044,1047,1051,1054,1058,1061,
1065,1069,1072,1076,1079,1083,1087,1090,1094,1098,1101,1105,1109,1112,1116,1120,1124,1127,1131,1135,1139,
1143,1146,1150,1154,1158,1162,1166,1170,1174,1178,1182,1186,1189,1193,1197,1201,1206,1210,1214,1218,1222,
1226,1230,1234,1238,1242,1247,1251,1255,1259,1263,1268,1272,1276,1280,1285,1289,1293,1298,1302,1306,1311,
1315,1320,1324,1328,1333,1337,1342,1346,1351,1355,1360,1365,1369,1374,1378,1383,1388,1392,1397,1402,1406,
1411,1416,1421,1425,1430,1435,1440,1444,1449,1454,1459,1464,1469,1474,1479,1484,1489,1494,1499,1504,1509,
1514,1519,1524,1529,1534,1539,1545,1550,1555,1560,1565,1571,1576,1581,1587,1592,1597,1603,1608,1613,1619,
1624,1630,1635,1641,1646,1652,1657,1663,1668,1674,1679,1685,1691,1696,1702,1708,1714,1719,1725,1731,1737,
1742,1748,1754,1760,1766,1772,1778,1784,1790,1796,1802,1808,1814,1820,1826,1832,1838,1845,1851,1857,1863,
1869,1876,1882,1888,1895,1901,1907,1914,1920,1927,1933,1940,1946,1953,1959,1966,1972,1979,1986,1992,1999,
2006,2012,2019,2026,2033,2040,2046,2053,2060,2067,2074,2081,2088,2095,2102,2109
};
uint16_t mapFreq(uint16_t input) {
  return pgm_read_word_near(freqTable + input);
}

void audioOn() {
  // Set up PWM to 31.25kHz, phase accurate
  TCCR2A = _BV(COM2A1) | _BV(WGM20);
  TCCR2B = _BV(CS20);
  TIMSK2 = _BV(TOIE2);
}


void setup() {
  Serial.begin(9600); 
  pinMode(PWM_PIN,OUTPUT);
  audioOn();
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  int pwmv = analogRead(SYNC_CONTROL)+(analogRead(SYNC_OFFSET)/5-205);
    if (pwmv > 1023) pwmv = 1023;
        if (pwmv < 0) pwmv = 0;
  Serial.print("pwmv: ");      
  Serial.print(pwmv);      
  Serial.print("\t");      
  syncPhaseInc = mapFreq(pwmv);

  Serial.print("syncPhaseInc: ");      
  Serial.print(syncPhaseInc);      
  Serial.print("\t");      

  grainPhaseInc  = mapFreq(analogRead(GRAIN_FREQ_CONTROL)) * 2;
  if (grainPhaseInc < 620) grainPhaseInc = 620;
  Serial.print("  grainPhaseInc: ");      
  Serial.print(  grainPhaseInc);      
  Serial.print("\t");      
  grainDecay     = 10 / 8;
  Serial.print("  grainDecay: ");      
  Serial.print(  grainDecay);      
  Serial.print("\t");      
  grain2PhaseInc = mapFreq(analogRead(GRAIN2_FREQ_CONTROL)) * 4;
  Serial.print("grain2PhaseInc: ");      
  Serial.print(grain2PhaseInc);      
  Serial.print("\t");      
  grain2Decay    = 10 / 4;
  Serial.print("grain2Decay: ");      
  Serial.print(grain2Decay);      
  Serial.print("\t");      
  Serial.print("\n");      
}

SIGNAL(PWM_INTERRUPT)
{

  
  uint8_t value;
  uint16_t output;

  syncPhaseAcc += syncPhaseInc;
  if (syncPhaseAcc < syncPhaseInc) {
    // Time to start the next grain
    grainPhaseAcc = 0;
    grainAmp = 0x7fff;
    grain2PhaseAcc = 0;
    grain2Amp = 0x7fff;
    LED_PORT ^= 1 << LED_BIT; // Faster than using digitalWrite
  }
  
  // Increment the phase of the grain oscillators
  grainPhaseAcc += grainPhaseInc;
  grain2PhaseAcc += grain2PhaseInc;

  // Convert phase into a triangle wave
  value = (grainPhaseAcc >> 7) & 0xff;
  if (grainPhaseAcc & 0x8000) value = ~value;
  // Multiply by current grain amplitude to get sample
  output = value * (grainAmp >> 8);

  // Repeat for second grain
  value = (grain2PhaseAcc >> 7) & 0xff;
  if (grain2PhaseAcc & 0x8000) value = ~value;
  output += value * (grain2Amp >> 8);

  // Make the grain amplitudes decay by a factor every sample (exponential decay)
  grainAmp -= (grainAmp >> 8) * grainDecay;
  grain2Amp -= (grain2Amp >> 8) * grain2Decay;

  // Scale output to the available range, clipping if necessary
  output >>= 9;
  if (output > 255) output = 255;

  // Output to PWM (this is faster than using analogWrite)  
  PWM_VALUE = output;
}

