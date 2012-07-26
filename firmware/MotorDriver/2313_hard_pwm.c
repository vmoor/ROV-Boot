#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "2313_hard_pwm.h"


void init_pwm (void)
{  
  
  // Timer 0 PWM Init

  TCCR0A = (1 << WGM00)
  		 | (1 << WGM01)
		 | (1 << COM0A1)
		 | (1 << COM0B1);
  TCCR0B = (0 << WGM02)
  		 | (1 << CS00)
		 | (1 << CS01)
		 | (0 << CS02);


  // OC0A PWM Port

  DDRB   |= (1 << PB2);

  OCR0A = 0;


  // OC0B PWM Port
  
  DDRD   |= (1 << PD5);

  OCR0B = 0;
  

  // Timer 1 PWM Init
/*
  TCCR1A = (1 << WGM10)
  		 | (0 << WGM11)
		 | (1 << COM1A1)
		 | (1 << COM1B1);
  TCCR1B = (1 << WGM12)
  		 | (0 << WGM13)
		 | (1 << CS10)
		 | (0 << CS11)
		 | (0 << CS12);


  // OC1A PWM Port

  DDRB   |= (1 << PB3);
  		 
  OCR1AL = 0;


  // OC1B PWM Port

  DDRB   |= (1 << PB4);
  		 
  OCR1BL = 0;
*/
}
