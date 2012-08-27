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

#define UART_MAXSTRLEN 12

volatile uint8_t uart_str_complete = 0;     // 1 .. String komplett empfangen
volatile uint8_t uart_str_count = 0;
volatile char uart_string[UART_MAXSTRLEN + 1] = "";



void init_digital();         // Ports initialisieren (eigene Funktion)  
void init_timer();           // Timer initialisieren (eigene Funktion)
void init_interrupt();       // Interrupt initialisieren
void USART_Init();


volatile int max_span_for_motor = 0x00;
volatile int geschw = 0;
char geschw_arr[3]; 


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
	    PORTD |= (1<<PD1);
//******************************************
	    OCR0B = 0x00;
	    PORTB |= (1<<PB0); 
	    PORTD |= (1<<PD6);

	while(1)
	{
	
		if (uart_str_complete == 1) // falls der String komplett empfangen wurde
		{
				
			//falls motortreiber mit ID M1
			if (uart_string[0] == 'M' && uart_string[1] == '1')
			{
				
				
				if (uart_string[10] == '0')
				{
					max_span_for_motor = 0xFF; 
				}
				else
				{
					if(uart_string[11] == '0') // 10 V
					{
						max_span_for_motor = 230; // Spannung auf 9 V reduzieren
					}
					else if(uart_string[11] == '1') // 11 V
					{
						max_span_for_motor = 209;
					}
					else if(uart_string[11] == '2') // 12 V
					{
						max_span_for_motor = 192;
					}
					else
					{
						max_span_for_motor = 100 ;
					}
					
				}

				




				geschw_arr[0] = uart_string[3];
				geschw_arr[1] = uart_string[4];
				geschw_arr[2] = uart_string[5]; 

				geschw = atoi(geschw_arr);

				OCR0B = ((max_span_for_motor*geschw) / 100);

				geschw_arr[0] = uart_string[7];
				geschw_arr[1] = uart_string[8];
				geschw_arr[2] = uart_string[9];

				geschw = atoi(geschw_arr);

				OCR0A = ((max_span_for_motor*geschw) / 100);
				
				//Motor M2
				//				
				if(uart_string[6] == 'V')
				{
					 PORTB |= (1<<PB1); 
	    			 PORTD &= ~(1<<PD1);
				}
				else if(uart_string[6] == 'R')
				{
					  PORTB &= ~(1<<PB1); 
	   				  PORTD |= (1<<PD1);
				}
				
				// Motor M1
			    if(uart_string[2] == 'V')
				{
					PORTB &= ~(1<<PB0); 
	    			PORTD |= (1<<PD6);
				}
				else if(uart_string[2] == 'R')
				{
					PORTB |= (1<<PB0); 
	    			PORTD &= (1<<PD6);
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




