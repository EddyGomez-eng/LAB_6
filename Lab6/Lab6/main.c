#define F_CPU 16000000UL                 // Frecuencia del microcontrolador

#include <avr/io.h>                      // Registros del ATmega328P
#include <util/delay.h>                  // Retardos
#include <stdlib.h>                      // Permite usar itoa()
#include "uart.h"                        // Librería UART creada

void ADC_init(void)                      // Configura el ADC
{
    DDRC &= ~(1 << PC0);                 // A0 como entrada
    PORTC &= ~(1 << PC0);                // Desactiva pull-up interno en A0

    ADMUX = (1 << REFS0);                // Referencia AVcc y canal ADC0/A0

    ADCSRA = (1 << ADEN) |               // Habilita el ADC
             (1 << ADPS2) |              // Prescaler 128
             (1 << ADPS1) |
             (1 << ADPS0);

    ADCSRB = 0x00;                       // Conversión normal
    DIDR0 |= (1 << ADC0D);               // Desactiva entrada digital en A0
}

uint16_t ADC_read(void)                  // Lee el valor del potenciómetro
{
    ADMUX = (1 << REFS0);                // Asegura AVcc y canal A0

    ADCSRA |= (1 << ADSC);               // Inicia conversión ADC
    while (ADCSRA & (1 << ADSC));        // Espera a que termine

    uint8_t bajo = ADCL;                 // Lee primero la parte baja
    uint8_t alto = ADCH;                 // Lee después la parte alta

    return ((uint16_t)alto << 8) | bajo; // Une ambos registros
}

char UART_Rx_Limpio(void)                // Recibe datos ignorando Enter
{
    char dato;                           // Guarda el dato recibido

    do                                   // Repite si recibe Enter
    {
        dato = UART_Rx();                // Lee carácter desde la terminal
    }
    while (dato == '\r' || dato == '\n'); // Ignora Enter y salto de línea

    return dato;                         // Devuelve carácter válido
}

int main(void)                           // Programa principal
{
    DDRB = 0xFF;                         // Puerto B como salida para LEDs
    PORTB = 0x00;                        // LEDs apagados al inicio

    UART_Init();                         // Inicializa comunicación UART
    ADC_init();                          // Inicializa lectura analógica en A0

    while (1)                            // Ciclo infinito
    {
        cadena("\r\nMENU:\r\n");        // Muestra el menú
        cadena("1. Leer Potenciometro\r\n"); // Opción para leer ADC
        cadena("2. Enviar ASCII\r\n");  // Opción para enviar ASCII
        cadena("Seleccione opcion:\r\n"); // Pide opción al usuario

        char opcion = UART_Rx_Limpio();  // Lee opción ignorando Enter

        if (opcion == '1')               // Si elige leer potenciómetro
        {
            uint16_t valor = ADC_read(); // Lee valor ADC de A0
			
			//PORTB = valor / 4;   // Convierte 0-1023 a 0-255 y lo muestra en LEDs

            char buffer[10];             // Guarda el valor convertido a texto

            itoa(valor, buffer, 10);     // Convierte número a texto decimal

            cadena("Valor ADC: ");       // Mensaje antes del valor
            cadena(buffer);              // Imprime valor del potenciómetro
            cadena("\r\n");             // Salto de línea

            // No se modifica PORTB para que el potenciómetro no altere los LEDs
        }

        else if (opcion == '2')          // Si elige enviar ASCII
        {
            cadena("Ingrese un caracter:\r\n"); // Pide carácter

            char dato = UART_Rx_Limpio(); // Lee carácter ignorando Enter

            cadena("Caracter recibido: "); // Confirma el carácter
            UART_Tx(dato);               // Devuelve el carácter recibido
            cadena("\r\n");             // Salto de línea

            PORTB = dato;                // Muestra ASCII recibido en LEDs
        }

        else                             // Si la opción no existe
        {
            cadena("Opcion invalida\r\n"); // Mensaje de error
        }

        _delay_ms(300);                  // Pausa antes de repetir el menú
    }
}