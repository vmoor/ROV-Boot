#include "usart.h"
#include <avr/io.h>
#include <util/delay.h>






int main( void )
{

	_delay_ms(500);
     USART_Init();
	 while(1)
	 {
	 USART_Transmit_Char('a');

	 _delay_ms(100);
	USART_Transmit_String("Hallo Welt\n");
	_delay_ms(100);

	}

	 
}



