/*
Biblioteca de manejo de UARTs en micros ATmega
MyEP 2017
*/
#include <stdio.h>

uint8_t mi_UART_Init0(uint16_t brate,uint8_t Txinterr,uint8_t Rxinterr);
int mi_putc0(char c, FILE *stream);
int mi_getc0(FILE *stream);

// Las 2 definiciones siguientes son para usar getc y putc para E/S de caracteres por UART
#define getc() mi_getc0(&uart_io)		// redefine la primitiva de entrada como  función recibir por UART
#define putc(x) mi_putc0(x, &uart_io)		// redefine la primitiva de salida como función transmitir por UART
