#include "timer_uno.h"
#include <util/delay.h>
#include <time.h>
#include <stdlib.h>
int main(int argc, char const *argv[]) {
  DDRB |= (1<<DDB1) | (1<<DDB2) | (1<<DDB0);
  confModoSalidas_TC(2, 2);
  interrupciones_TC(0,0,0,0);
  confPrescaler_TC(1000);
  confModo_TC(14);
  // conf_PWM_14_TC1(1000, 100, 900);
  sei();
  srand(time(NULL));
  setTP(2000);

  setDutyB(100);
  while (1) {
      for (size_t i = 0; i < 2001; i+=100) {
        setDutyA(i);
        _delay_ms(4000);
        PORTB ^= (1<<PORTB0);
      }

      _delay_ms(1000);
  }

  return 0;
}
