// Copyright (c) 2015, Burak Aybar
// All rights reserved.

#include <digitalWriteFast.h>

int16_t lft, rht;
uint16_t ms;
static int16_t lTotal = 0;
static int16_t rTotal = 0;
static int16_t lastlTotal = 0;
static int16_t lastrTotal = 0;
long en_lastCall = 0;
volatile int16_t en_lft_ticks = 0;
volatile int16_t en_rht_ticks = 0;
volatile uint16_t en_counter = 0;
volatile bool en_error = false;
char en_lApin;
char en_lBpin;
char en_rApin;
char en_rBpin;

void setup_encoder( char lftPinA, char lftPinB, char rhtPinA, char rhtPinB )
{
  en_init_pin( &en_lApin, lftPinA);
  en_init_pin( &en_lBpin, lftPinB);
  en_init_pin( &en_rApin, rhtPinA);
  en_init_pin( &en_rBpin, rhtPinB);

  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  OCR2A = 31;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22);   
  TIMSK2 |= (1 << OCIE2A);
  sei();
}

void en_init_pin( char *pin, char value)
{
  *pin = value;
  pinMode(value, INPUT);
  digitalWrite( value, HIGH);
}

int get_ticks_M1()
{
  return  en_lft_ticks;
}
int get_ticks_M2()
{
  return  en_rht_ticks;
}

void clear_ticks()
{
  cli();
  lastlTotal=0;
  lastrTotal=0;
  rht=0;
  lft=0;
  en_lft_ticks = en_rht_ticks = en_counter = 0;
  en_error = false;
  en_lastCall = millis();
  sei();
}

ISR(TIMER2_COMPA_vect)
{
  // this routine gets called once every 1 millisecond
  en_counter++;

  static char lastLA = 0;
  static char lastLB = 0;
  static char lastRA = 0;
  static char lastRB = 0;

  en_process(en_lApin, en_lBpin, &lastLA, &lastLB, &en_lft_ticks);
  en_process(en_rApin, en_rBpin, &lastRA, &lastRB, &en_rht_ticks);
}

void en_process( char Apin, char Bpin, char *lastA, char *lastB, volatile int16_t *ticks )
{
  char A = (digitalReadFast( Apin) == HIGH) ? 1 : 0;
  char B = (digitalReadFast( Bpin) == HIGH) ? 1 : 0;
  char lA = *lastA;
  char lB = *lastB;
  char dA = A!=lA;
  char dB = B!=lB;

  if( dA && dB )
  {
    // both should not change at the same time
    en_error = true;
  }
  else if ( dA || dB )
  {
    if (A^lB) 
    {
      *ticks += 1;
    }
    else if(B^lA)
    {
      *ticks -= 1;
    }
    if(Apin ==  en_lApin){
      lastlTotal += lft;
    }
    else if(Apin == en_rApin){
      lastrTotal += rht;
    }
  }
  *lastA = A; 
  *lastB = B;
}
