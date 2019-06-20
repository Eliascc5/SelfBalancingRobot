#include "timer_uno.h"
#include <util/delay.h>
#include <time.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
ISR(TIMER0_COMPA_vect){
  _delay_ms(100);
  PORTD ^= (1<<PORTD4);
  TIFR0 &=~ (1<<OCF0A); //apaga flag
}
int main(int argc, char const *argv[]) {
  DDRD |= (1<<PORTD6) | (1<<PORTD4);
  // PORTD |= (1<<PORTD4);
  confModo_T8(2);
  confModoSalidas_T8(1, 0);
  interrupciones_T8(1,0,0);
  confPrescaler_T8(10);

  // // conf_PWM_14_TC1(1000, 100, 900);
  setDutyA8(10);
  sei();
  // setDutyB8(900);
  // TCCR0A |= (1<<WGM01);
  // TCCR0A |= (1<<COM0A0);
  // TCCR0B |= (5<<CS00);
  // OCR0A = (uint8_t)(255); //Numero hasta el cual cuenta para llegar al
  // sOLO PUEDO CONTAR HASTA 256 -> LAS FRECUENCIAS SON MAS CHICAS.
  while (1) {
      _delay_ms(1);
  }

  return 0;
}
