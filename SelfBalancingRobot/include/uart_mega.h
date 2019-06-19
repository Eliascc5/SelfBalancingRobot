#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>

void conf_UART(unsigned int baud, unsigned int intRx, unsigned int intTx);
int mi_putc(char c, FILE *stream);
int mi_getc(FILE *stream);
// Redefinicion de las funciones para recibir/transmitir caracteres por UART
#define fgetc() mi_getc(&uart_io);
#define fputc() mi_putc(x, &uart_io);
// Declaracion parametro tipo stream de E/S para igualar los parametros de stdio
FILE uart_io = FDEV_SETUP_STREAM(mi_putc, mi_getc, _FDEV_SETUP_RW);
int mi_putc(char c, FILE *stream){
  /*
  Redefinicon de la salida fputc() de la libreria stdio
  para poder usarla como UART
  :param c: caracter a enviar
  :param sream: parametro necesario para redefinir la libreria stdio
  return 0:
  */
  while (!(UCSR0A&(1<<UDRE0))){
    _delay_us(1); //Espero a que se desocupe el buffer
  }
    UDR0 = c; //Pongo el nuevo dato a transmitir
    return 0;
}
int mi_getc(FILE *stream){
  /*
  Redefinicon de la salida fgetc() de la libreria stdio
  para poder usarla como UART
  :param sream: parametro necesario para redefinir la libreria stdio
  return UDR0: caracter recibido
  */
  while (!(UCSR0A&(1<<RXC0))){
    _delay_ms(1);//Espero a que se desocupe el buffer
  }
    return UDR0; //leo el caracter
}
void conf_UART(unsigned int baud, unsigned int intRx, unsigned int intTx){
  /*
  Funcion para configurar UART
  :param BAUD: velocidad de transmicion en baudios
  :param intRX: entero para habilitar interrupciones por recepcion
  :param intTX: entero para habilitar interrupciones por transmision
  */
  // ------------------------------------------------
  // -           Parametros de la comunicacion      -
  // ------------------------------------------------
  UBRR0 = F_CPU/16/baud-1; //Conf baudrate
  UCSR0A &=~ (1<<U2X0);     //velocidad simple (1 para doble)
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);   //Habilita recepcion y  transmicion
  UCSR0C |= (1<<USBS0) | (3<<UCSZ00);    // 2 bit stop (0 -> 1 bit)
                                        // 8 bit dato (pongo en 1 ambos bit 00 y 01)

  if (intRx) {
    UCSR0A &=~ (1<<RXC0) ;  //Apago el flag de recepcion completa
    UCSR0B |= (1<<RXCIE0);  //Habilito interrupción RX
  }

  if (intTx) {
    UCSR0A &=~ (1<<TXC0);   //Apago el flag de transmicion completa
    UCSR0B |= (1<<TXCIE0);  //Habilito interrupción RX
  }
}
