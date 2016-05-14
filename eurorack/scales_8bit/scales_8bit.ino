
int PWM_OUT = 9;

void setup()
{
  Serial.begin(9600);
/**********************************************************************************/
// Set pwm clock divider
/**********************************************************************************/ 
   TCCR1B &= ~(1 << CS12);
   TCCR1B  |=   (1 << CS11);
   TCCR1B &= ~(1 << CS10);                               

/**********************************************************************************/
// Set pwm resolution  to mode 7 (10 bit)
/**********************************************************************************/ 

  TCCR1B &= ~(1 << WGM13);    // Timer B clear bit 4
  TCCR1B |=  (1 << WGM12);    // set bit 3
  
  TCCR1A |= (1 << WGM11);    //  Timer A set bit 1
  TCCR1A |= (1 << WGM10);    //  set bit 0
  
  
  /*
  1 Step = 16;    
    
  */

  pinMode(PWM_OUT, OUTPUT);
}


int pitch_pwm = 0;
int prev_pitch_pwm = 0;
int dir = 1;
int step = 1 ;

int min = 0;

void loop()
{
  
  pitch_pwm += (dir * step);
  if (pitch_pwm >= 1024) {
       pitch_pwm = 512;
//       pitch_pwm = prev_pitch_pwm;
//    dir = -1;
  }

  if (pitch_pwm < 0) {
    pitch_pwm = 0;
    dir = 1;
  }
  analogWrite(PWM_OUT, pitch_pwm);
//  analogWrite(PWM_OUT, 960);
//  Serial.println(pitch_pwm);
//  delay(500);
//  analogWrite(PWM_OUT, 600);
//  Serial.println(pitch_pwm);
//  analogWrite(PWM_OUT, 256);
//  delay(5);
//  analogWrite(PWM_OUT, 512);
  prev_pitch_pwm = pitch_pwm;
}
