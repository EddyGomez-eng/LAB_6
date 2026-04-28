#define F_CPU 16000000UL                         // Indica que el micro trabaja a 16 MHz

#include "uart.h"                                // Incluye las declaraciones de nuestra propia librería

void UART_Init(void)                             // Inicia la definición de la función de configuración UART
{
	UBRR0H = 0;                                  // Coloca en 0 la parte alta del baud rate
	UBRR0L = 103;                                // Coloca 103 en la parte baja para trabajar a 9600 baudios

	UCSR0B = (1 << TXEN0) | (1 << RXEN0);        // Habilita el transmisor y el receptor de la UART
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);      // Configura 8 bits de datos, sin paridad y 1 bit de stop
}

void UART_Tx(char dato)                          // Inicia la función que enviará un carácter
{
	while (!(UCSR0A & (1 << UDRE0)));            // Espera hasta que el registro de envío quede libre
	UDR0 = dato;                                 // Carga el carácter en UDR0 para transmitirlo
}

uint8_t UART_HayDato(void)                       // Inicia la función que revisa si ya llegó algo
{
	return (UCSR0A & (1 << RXC0));               // Devuelve un valor distinto de 0 si la UART ya recibió un dato
}

char UART_Rx(void)                               // Inicia la función que lee un carácter recibido
{
	while (!(UCSR0A & (1 << RXC0)));             // Espera aquí hasta que entre un dato por la UART
	return UDR0;                                 // Devuelve el carácter que llegó al registro de recepción
}

void cadena(char txt[])                          // Inicia la función que enviará una cadena completa
{
	uint8_t i = 0;                               // Crea un índice que empezará en la primera posición del texto

	while (txt[i] != '\0')                       // Recorre la cadena mientras no llegue al final del texto
	{
		UART_Tx(txt[i]);                         // Envía el carácter que está en la posición actual
		i++;                                     // Avanza a la siguiente letra de la cadena
	}
}