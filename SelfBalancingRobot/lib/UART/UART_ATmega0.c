	/*
UART EN MODO ASINCRONO(FULL-duplex), con transmision de 8 bit por caracter, 1 bit de stop
----------------
Los registros de datos a transmitir y recibir en este uC comparten la misma direccion,
la del registro UDR0 (Registro de datos de entrada y salida)

UCSR0A: Registro 0A de estado y control de la UART
   	0B: Registro 0B de estado y control de la UART
	 	0C: Registro 0C de estado y control de la UART
	*/
	#ifndef F_CPU
	#define F_CPU 16000000
	#endif
	//#include "UART_ATmega0.h"
	#include <stdint.h>
	#include <stdio.h>    //getc(), putc()
	#include <avr/io.h>
  #include <util/delay.h>


	uint8_t mi_UART_Init0(uint16_t brate, uint8_t Txinterr,uint8_t Rxinterr){
			UBRR0 = F_CPU/16/brate-1;						// Configura baudrate. Ver en sección UART de datasheet(PAG 173)
			//UCSR0A|=(1<<U2X0);

			UCSR0B = (1<<RXEN0)|(1<<TXEN0);			// Habilita bits TXEN0 y RXEN0 (PINES)
			UCSR0C = (1<<USBS0)|(3<<UCSZ00);		// USBS0=1 2 bits stop, UCSZxx=3 8 bits (tamaño del caracter)
			if (Txinterr){
			  UCSR0A &=~ (1<<TXC0);   //Apago el flag de transmicion completa
				UCSR0B |= (1<<TXCIE0);	// Interrupcion Tx UART0
			}
			if (Rxinterr){
			  UCSR0A &=~ (1<<RXC0) ;  //Apago el flag de recepcion completa
				UCSR0B |= (1<<RXCIE0);	// Interrupcion Rx UART0
			}
			return 1;
	}

	int mi_putc0(char c, FILE *stream){  // Función para transmitir caracter por UART
			while(!(UCSR0A & (1<<UDRE0))){
				_delay_us(1);
			} // Espera mientras el bit UDRE0=0 (buffer de transmisión ocupado)
			UDR0 = c;												// Cuando se desocupa, UDR0 puede recibir el nuevo dato c a trasmitir
			return 0;
	}

	int mi_getc0(FILE *stream){      // Función para recibir caracter por UART
			while (!(UCSR0A & (1<<RXC0))){
			_delay_us(1);
			}// Espera mientras el bit RXC0=0 (recepción incompleta)
			return UDR0;							// Cuando se completa, se lee UDR0
	}
