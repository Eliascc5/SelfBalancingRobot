/*

Declaracion de los pines GPIO del micro atmega328p.
DDRx -> Direccionamiento
PORTx ->Estado

*/
#include <avr/io.h>   //Definiciones de puertos y registros

#define B5_encendido (PORTB |=(1<<PORTB5))
#define B4_encendido (PORTB |=(1<<PORTB4))
#define B3_encendido (PORTB |=(1<<PORTB3))
#define B2_encendido (PORTB |=(1<<PORTB2))
#define B1_encendido (PORTB |=(1<<PORTB1))
#define B0_encendido (PORTB |=(1<<PORTB0))

#define B5_apagado (PORTB &=~(1<<PORTB5))
#define B4_apagado (PORTB &=~(1<<PORTB4))
#define B3_apagado (PORTB &=~(1<<PORTB3))
#define B2_apagado (PORTB &=~(1<<PORTB2))
#define B1_apagado (PORTB &=~(1<<PORTB1))
#define B0_apagado (PORTB &=~(1<<PORTB0))

#define B5_entrada (DDRB &=~(1<<DDB5))
#define B4_entrada (DDRB &=~(1<<DDB4))
#define B3_entrada (DDRB &=~(1<<DDB3))
#define B2_entrada (DDRB &=~(1<<DDB2))
#define B1_entrada (DDRB &=~(1<<DDB1))
#define B0_entrada (DDRB &=~(1<<DDB0))

#define B5_salida (DDRB |=(1<<DDB5))
#define B4_salida (DDRB |=(1<<DDB4))
#define B3_salida (DDRB |=(1<<DDB3))
#define B2_salida (DDRB |=(1<<DDB2))
#define B1_salida (DDRB |=(1<<DDB1))
#define B0_salida (DDRB |=(1<<DDB0))

#define B5_Toggle (PORTB^=(1<<PORTB5))
#define B4_Toggle (PORTB^=(1<<PORTB4))
#define B3_Toggle (PORTB^=(1<<PORTB3))
#define B2_Toggle (PORTB^=(1<<PORTB2))
#define B1_Toggle (PORTB^=(1<<PORTB1))
#define B0_Toggle (PORTB^=(1<<PORTB0))

#define B5_test (PINB & (1<<PINB5))
#define B4_test (PINB & (1<<PINB4))
#define B3_test (PINB & (1<<PINB3))
#define B2_test (PINB & (1<<PINB2))
#define B1_test (PINB & (1<<PINB1))
#define B0_test (PINB & (1<<PINB0))

/*
PORT D
*/

#define D7_encendido (PORTD |=(1<<PORTD7))
#define D6_encendido (PORTD |=(1<<PORTD6))
#define D5_encendido (PORTD |=(1<<PORTD5))
#define D4_encendido (PORTD |=(1<<PORTD4))
#define D3_encendido (PORTD |=(1<<PORTD3))
#define D2_encendido (PORTD |=(1<<PORTD2))
#define D1_encendido (PORTD |=(1<<PORTD1))
#define D0_encendido (PORTD |=(1<<PORTD0))

#define D7_apagado (PORTD &=~(1<<PORTD7))
#define D6_apagado (PORTD &=~(1<<PORTD6))
#define D5_apagado (PORTD &=~(1<<PORTD5))
#define D4_apagado (PORTD &=~(1<<PORTD4))
#define D3_apagado (PORTD &=~(1<<PORTD3))
#define D2_apagado (PORTD &=~(1<<PORTD2))
#define D1_apagado (PORTD &=~(1<<PORTD1))
#define D0_apagado (PORTD &=~(1<<PORTD0))

#define D7_entrada (DDRD &=~(1<<DDD7))
#define D6_entrada (DDRD &=~(1<<DDD6))
#define D5_entrada (DDRD &=~(1<<DDD5))
#define D4_entrada (DDRD &=~(1<<DDD4))
#define D3_entrada (DDRD &=~(1<<DDD3))
#define D2_entrada (DDRD &=~(1<<DDD2))
#define D1_entrada (DDRD &=~(1<<DDD1))
#define D0_entrada (DDRD &=~(1<<DDD0))

#define D7_salida (DDRD |=(1<<DDD7))
#define D6_salida (DDRD |=(1<<DDD6))
#define D5_salida (DDRD |=(1<<DDD5))
#define D4_salida (DDRD |=(1<<DDD4))
#define D3_salida (DDRD |=(1<<DDD3))
#define D2_salida (DDRD |=(1<<DDD2))
#define D1_salida (DDRD |=(1<<DDD1))
#define D0_salida (DDRD |=(1<<DDD0))

#define D7_Toggle (PORTD^=(1<<PORTD7))
#define D6_Toggle (PORTD^=(1<<PORTD6))
#define D5_Toggle (PORTD^=(1<<PORTD5))
#define D4_Toggle (PORTD^=(1<<PORTD4))
#define D3_Toggle (PORTD^=(1<<PORTD3))
#define D2_Toggle (PORTD^=(1<<PORTD2))
#define D1_Toggle (PORTD^=(1<<PORTD1))
#define D0_Toggle (PORTD^=(1<<PORTD0))

#define D7_test (PIND & (1<<PINB7))
#define D6_test (PIND & (1<<PINB6))
#define D5_test (PIND & (1<<PINB5))
#define D4_test (PIND & (1<<PINB4))
#define D3_test (PIND & (1<<PINB3))
#define D2_test (PIND & (1<<PINB2))
#define D1_test (PIND & (1<<PINB1))
#define D0_test (PIND & (1<<PINB0))

/*
Puertos C --> analogicos
*/

#define C5_encidido (PORTC |=(1<<PORTC5))
#define C4_encidido (PORTC |=(1<<PORTC4))
#define C3_encidido (PORTC |=(1<<PORTC3))
#define C2_encidido (PORTC |=(1<<PORTC2))
#define C1_encidido (PORTC |=(1<<PORTC1))
#define C0_encidido (PORTC |=(1<<PORTC0))

#define C5_apagado (PORTC &=~(1<<PORTC5))
#define C4_apagado (PORTC &=~(1<<PORTC4))
#define C3_apagado (PORTC &=~(1<<PORTC3))
#define C2_apagado (PORTC &=~(1<<PORTC2))
#define C1_apagado (PORTC &=~(1<<PORTC1))
#define C0_apagado (PORTC &=~(1<<PORTC0))

#define C5_entrada (DDRC &=~(1<<DDC5))
#define C4_entrada (DDRC &=~(1<<DDC4))
#define C3_entrada (DDRC &=~(1<<DDC3))
#define C2_entrada (DDRC &=~(1<<DDC2))
#define C1_entrada (DDRC &=~(1<<DDC1))
#define C0_entrada (DDRC &=~(1<<DDC0))

#define C5_salida (DDRC |=(1<<DDC5))
#define C4_salida (DDRC |=(1<<DDC4))
#define C3_salida (DDRC |=(1<<DDC3))
#define C2_salida (DDRC |=(1<<DDC2))
#define C1_salida (DDRC |=(1<<DDC1))
#define C0_salida (DDRC |=(1<<DDC0))
