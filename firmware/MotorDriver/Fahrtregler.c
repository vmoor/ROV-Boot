#include <avr/io.h>
#include <avr/interrupt.h> 
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "2313_hard_pwm.h"

#define BAUD 9600
#include <util/setbaud.h>

#define UART_MAXSTRLEN 8

volatile uint8_t uart_str_complete = 0;     // 1 .. String komplett empfangen
volatile uint8_t uart_str_count = 0;
volatile char uart_string[UART_MAXSTRLEN + 1] = "";



void init_digital();         // Ports initialisieren (eigene Funktion)  
void init_timer();           // Timer initialisieren (eigene Funktion)
void init_interrupt();      // Interrupt initialisieren
void USART_Init();
;

volatile int8_t max_span_for_motor = 0x00;
volatile int8_t schritt_0_5_v = 0x00;


int main(void)
{

    init_digital();
	init_timer();
	init_pwm();	
	USART_Init();

	// bei Prototypen OCR0A auf OCR0B ändern
    //PD1 und PB1 richtung von M2 pwm2  PB2(OCR0A) - Geschwindigkeit
	//PD6 und PB0 richtung von M1 pwm1  PD5(OCR0B) - Geschwindigkeit
	 

	sei(); //Interrupts aktivieren
	

//******************************************
		OCR0A = 0x00;
	    PORTB |= (1<<PB1); 
	    PORTD &= ~(1<<PD1);
//******************************************
	    OCR0B = 0x00;
	    PORTB |= (1<<PB0); 
	    PORTD &= ~(1<<PD6);

	while(1)
	{
	
		if (uart_str_complete == 1) // falls der String komplett empfangen wurde
		{

			//falls motortreiber mit ID M1
			if (uart_string[0] == 'M' && uart_string[1] == '1')
			{
				
				
				if (uart_string[6] == '0')
				{
					schritt_0_5_v = 255; 
				}
				else

				{
					//TODO spanunganpassung an 9 V
					max_span_for_motor = 0x5F;
				}

				
		
				//Motor M2
				//
			//	max_span_for_motor = 0x00;
			//	for (int8_t i = 0; i <  uart_string[3] +1 ; i++)
				{
//					max_span_for_motor = max_span_for_motor + schritt_0_5_v;
				}

				OCR0A = 0x00 + (max_span_for_motor / 10 * (uart_string[3] +1));
				if(uart_string[2] == 'V')
				{
					 PORTB |= (1<<PB1); 
	    			 PORTD &= ~(1<<PD1);
				}
				else if(uart_string[2] == 'R')
				{
					  PORTB &= ~(1<<PB1); 
	   				  PORTD |= (1<<PD1);
				}
				
				// Motor M1
			    if(uart_string[2] == 'V')
				{

				}
				else if(uart_string[2] == 'R')
				{

				}



			}

			uart_str_complete = 0;
		}
			
	}
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
	//Timer 1 (8Bit)
 	TCCR1B |= (1<<CS11);  //Vorteiler auf CPU Takt/8
}




void USART_Init()
{

	//Baudrate setzen

	UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
		
  	UCSRB = (1<<RXEN) | (1<<RXCIE);       // UART RX und RX Interrupt einschalten
  	UCSRC = (3 << UCSZ0); // Asynchron 8N1 

	


}


ISR(USART_RX_vect)
{
  unsigned char nextChar;
 
  // Daten aus dem Puffer lesen
  nextChar = UDR;

  if(nextChar == 'n')
  {
	OCR0A = 0x5F;
  }
  


  if( uart_str_complete == 0 ) 	// wenn uart_string gerade in Verwendung, neues Zeichen verwerfen
  {
    // Daten werden erst in uart_string geschrieben, 
	//wenn nicht String-Ende/max Zeichenlänge erreicht ist/string gerade verarbeitet wird
    if( nextChar != '\n' &&
        nextChar != '\r' &&
        uart_str_count < UART_MAXSTRLEN ) 
	{
      uart_string[uart_str_count] = nextChar;
      uart_str_count++;
    }
    else 
	{
      uart_string[uart_str_count] = '\0';
      uart_str_count = 0;
      uart_str_complete = 1;
    }
  }
 
}




