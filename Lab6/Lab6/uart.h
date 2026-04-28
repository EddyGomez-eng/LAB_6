#ifndef UART_H_                                  // Evita que este archivo se incluya dos veces por error
#define UART_H_                                  // Marca de protección para este header

#include <avr/io.h>                              // Trae los registros del ATmega328P
#include <stdint.h>                              // Permite usar tipos como uint8_t

void UART_Init(void);                            // Declara la función que configura la UART
void UART_Tx(char dato);                         // Declara la función que envía un carácter
uint8_t UART_HayDato(void);                      // Declara la función que revisa si llegó un dato
char UART_Rx(void);                              // Declara la función que lee un carácter recibido
void cadena(char txt[]);                         // Declara la función que envía una cadena completa

#endif /* UART_H_ */                             // Cierra la protección del archivo