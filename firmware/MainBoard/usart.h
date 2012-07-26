
#define F_CPU 8000000 /* evtl. bereits via Compilerparameter definiert */
#define BAUD 19200

#include <avr/io.h>
#include <util/setbaud.h>

//UART initialisieren
void USART_Init();
//Einen Zeichen senden
void USART_Transmit_Char( unsigned char data );
//Ein String senden
void USART_Transmit_String (char *s);
