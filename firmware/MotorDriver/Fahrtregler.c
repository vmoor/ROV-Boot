#include <avr/io.h>
#include <avr/interrupt.h> 
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "2313_hard_pwm.h"

#ifndef F_CPU
#define F_CPU 4000000L
#endif


void init_digital();         // Ports initialisieren (eigene Funktion)  
void init_timer();           // Timer initialisieren (eigene Funktion)
void init_interrupt();      // Interrupt initialisieren

//Variablen fuer Motor 1
volatile uint16_t start_M1;
volatile uint16_t stop_M1;
volatile uint16_t impuls_M1;
volatile uint8_t flanke_M1;

//Variablen fuer Motor 2
volatile uint16_t start_M2;
volatile uint16_t stop_M2;
volatile uint16_t impuls_M2;
volatile uint16_t flanke_M2;

int main(void)
{
	init_interrupt();
    init_digital();
	init_timer();
	init_pwm();	

    //PD1 und PB1 richtung von M2 pwm2  PB2 - Geschwindigkeit
	//PD6 und PB0 richtung von M1 pwm1  PD5 - Geschwindigkeit
	 

	sei(); //Interrupts aktivieren
	
	while(1)
	{
	//*****************************TODO test delay**********************
	_delay_ms(1);

	//----------- Motor 1 Steuerung START -------------  
	  if(impuls_M1 == 0)
	  {
		
	    PORTB &= ~(1<<PB1); 
	    PORTD &= ~(1<<PD1); 
  
	  }
	  if ((impuls_M1 > 445) & (impuls_M1 < 700))
	  {
//******************************************
//beim Prototypen OCR0A auf OCR0B aendern
	    OCR0A = 700 - impuls_M1;
//******************************************
	    PORTB |= (1<<PB1); 
	    PORTD &= ~(1<<PD1); 
	   }
  
	  if ((impuls_M1 > 800) & (impuls_M1 < 1055))
	  {
//******************************************
//beim Prototypen OCR0A auf OCR0B aendern
	    OCR0A =  impuls_M1 - 800;
//******************************************
	    PORTB &= ~(1<<PB1);
	    PORTD |= (1<<PD1);
	  }
  
	  if ((impuls_M1 > 700) & (impuls_M1 < 800))
	  {
//******************************************
//beim Prototypen OCR0A auf OCR0B aendern
	    OCR0A = 0x5A;
//******************************************
	    PORTB |= (1<<PB1); 
	    PORTD |= (1<<PD1); 
	  }		 
	//----------- Motor 1 Steuerung ENDE -------------
		
	//----------- Motor 2 Steuerung START -------------  
	  if(impuls_M2 == 0)
	  {
		
	    PORTB &= ~(1<<PB0); 
	    PORTD &= ~(1<<PD6); 
  
	  }
	  if ((impuls_M2 > 445) & (impuls_M2 < 700))
	  {
//******************************************
//beim Prototypen OCR0B auf OCR0A aendern
	    OCR0B = 700 - impuls_M2;
//******************************************
	    PORTB |= (1<<PB0); 
	    PORTD &= ~(1<<PD6); 
	   }
  
	  if ((impuls_M2 > 800) & (impuls_M2 < 1055))
	  {
//******************************************
//beim Prototypen OCR0B auf OCR0A aendern
	    OCR0B =  impuls_M2 - 800;
//******************************************
	    PORTB &= ~(1<<PB0);
	    PORTD |= (1<<PD6);
	  }
  
	  if ((impuls_M2 > 700) & (impuls_M2 < 800))
	  {
//******************************************
//beim Prototypen OCR0B auf OCR0A aendern

	    OCR0B = 0x5A;        
//******************************************
	    PORTB |= (1<<PB0); 
	    PORTD |= (1<<PD6); 
	  }		 
	//----------- Motor 1 Steuerung ENDE -------------
			
	}
}

// Interrupt an INT0 auswerten
ISR(INT0_vect)
{
 	cli();
 	if (flanke_M1 == 1)
  	{
   		start_M1 = TCNT1;
   		MCUCR |= (1<<ISC01); //INT0 auf fallende Flanke stellen
   		MCUCR &= ~(1<<ISC00);
   		flanke_M1 = 0;
  	}
   	else
    {
	 	stop_M1 = TCNT1;
		if (stop_M1 > start_M1)
		{
	 		impuls_M1 = stop_M1 - start_M1;
     	}
		else
		{
			impuls_M1 = 0xFFFF - start_M1 + stop_M1;
		}
		MCUCR |= (1<<ISC00) | (1<<ISC01); //INT0 auf Steigende Flanke stellen (Löst Interrupt bei steigendet Flanke an INT0 aus)
     	flanke_M1 = 1;  
    } 
	sei();
 
}

// Interrupt an INT1 auswerten
ISR(INT1_vect)
{
 	cli();
 	if (flanke_M2 == 1)
  	{
   		start_M2 = TCNT1;
   		MCUCR |= (1<<ISC11); //INT1 auf fallende Flanke stellen
   		MCUCR &= ~(1<<ISC10);
   		flanke_M2 = 0;
  	}
   	else
    {
	 	stop_M2 = TCNT1;
	 	if (stop_M2 > start_M2)
		{
	 		impuls_M2 = stop_M2 - start_M2;
     	}
		else
		{
			impuls_M2 = 0xFFFF - start_M2 + stop_M2;
		}
     	MCUCR |= (1<<ISC10) | (1<<ISC11); //INT1 auf Steigende Flanke stellen (Löst Interrupt bei steigendet Flanke an INT1 aus)
     	flanke_M2 = 1;  
    } 
 	sei();
}

void init_digital(void)
/*Diese Funktion initialisiert die verwendeten Ports*/
{
  	DDRB |= (1<<DDB0) | (1<<DDB1);  
  	DDRD |= (1<<DDD1) | (1<<DDD6);
}

// Timer initialisieren
void init_timer(void)
{
	//Timer 1 (16Bit)
 	TCCR1B |= (1<<CS11);  //Vorteiler auf CPU Takt/8
}


// Interrupt auf INT0 und INT1 initialisieren
void init_interrupt(void)
{
	GIMSK  |= (1<<INT0) | (1<<INT1); //Interrupt von INT0 auf Enable
 	MCUCR |= (1<<ISC00) | (1<<ISC01) | (1<<ISC10) | (1<<ISC11); //INT0 auf Steigende Flanke stellen (Löst Interrupt bei steigendet Flanke an INT0 aus
}
