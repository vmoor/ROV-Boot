
#include "usart.h"



void USART_Init()
{

	//Baudrate setzen

	UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
		
  	UCSR0B = (1<<RXEN0)|(1<<TXEN0);       // UART TX und RX einschalten
  	UCSR0C = (1 << UCSZ01)|(1 << UCSZ00); // Asynchron 8N1 



}

//Einen Zeichen senden
void USART_Transmit_Char( unsigned char data )
{

	while ( !( UCSR0A & (1<<UDRE0)) ) //warten bisransmitterbuffer frei ist
	;

	UDR0 = data; 					  //Schreiben Daten in Buffer und senden
}

//Ein String senden
void USART_Transmit_String (char *s)
{
    while (*s)
    {   /* so lange *s != '\0' also ungleich dem "String-Endezeichen(Terminator)" */
        USART_Transmit_Char(*s);
        s++;
    }
}

/*
unsigned char USART_Receive( void )
{
	// Wait for data to be received 
	while ( !(UCSRnA & (1<<RXCn)) )
	;

	// Get and return received data from buffer 
	return UDRn;
}
*/
