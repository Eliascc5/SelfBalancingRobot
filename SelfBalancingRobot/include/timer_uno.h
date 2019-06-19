#include <math.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>

uint16_t prescalerValue_TC = 0;
/**
* CSn2 CSn1 CSn0
*  0    0    1    clkI/O/1 (No prescaling      (1<<CSn0)
*  0    1    0    clkI/O/8 (From prescaler)    (2<<CSn0)
*  0    1    1    clkI/O /64 (From prescaler)  (3<<CSn0)
*  1    0    0    clkI/O/256 (From prescaler)  (4<<CSn0)
*  1    0    1    clkI/O/1024 (From prescaler) (5<<CSn0)
*/

int confPrescaler_TC(float T) {
/**
*   Funcion que setear el prescalaer para un timer de 16bits dado
*   @param T: tiempo en ms que se quiere obtener
*   @returns 0/1 : 0 si todo funciono 1 si algo anda mal
*/
  T = (float)(T/1000.0);
  float aux = (float)(pow(2, 16)/F_CPU); //Maximo Tiempo en s que se puede llegar
  if      (T <= aux)      prescalerValue_TC = 1;
  else if (T <= 8 * aux)  prescalerValue_TC = 8;
  else if (T <= 64 * aux)  prescalerValue_TC = 64;
  else if (T <= 256 * aux)  prescalerValue_TC = 256;
  else if (T <= 1024 * aux)  prescalerValue_TC = 1024;
  else if (T > 1024 * aux) return 1; //Se puede probar si anda con return 2 para codificar el error
  TCCR1B &=~ (7<<CS10); //Coloco en 0 todo el registro TCCRXB (Reset de la configuracion)

  switch (prescalerValue_TC) {
    case 0: //timer off
          TCCR1B &=~ (7<<CS10); //Coloco en 0 todo el registro TCCRXB
          break;
    case 1:
          TCCR1B |= (1<<CS10);
          break;
    case 8:
          TCCR1B |= (2<<CS10);
          break;
    case 64:
          TCCR1B |= (3<<CS10);
          break;
    case 256:
          TCCR1B |= (4<<CS10);
          break;
    case 1024:
          TCCR1B |= (5<<CS10);
          break;
    default:
          return 1;
  }
return 0;
}

int confModo_TC(uint8_t modo) {
/**
* Función para selección del modo de uso de timer
* @param modo: modo del timer a usar 0-15
* @returns 0/1 : 0 si todo funciono 1 si algo anda mal
*/
    TCCR1A &=~ (3<<WGM10);
    TCCR1B &=~ (3<<WGM12);
    switch (modo) {
      case 0:
            TCCR1A &=~ (3<<WGM10);
            TCCR1B &=~ (3<<WGM12);
            break;
      case 1:
            TCCR1A |= (1<<WGM10);
            break;
      case 2:
            TCCR1A |= (1<<WGM11);
            break;
      case 3:
            TCCR1A |= (3<<WGM11);
            break;
      case 4:
            TCCR1B |= (1<<WGM12);
            break;
      case 5:
            TCCR1A |= (1<<WGM10);
            TCCR1B |= (1<<WGM12);
            break;
      case 6:
            TCCR1A |= (1<<WGM11);
            TCCR1B |= (1<<WGM12);
            break;
      case 7:
            TCCR1A |= (3<<WGM10);
            TCCR1B |= (1<<WGM12);
            break;
      case 8:
            TCCR1B |= (1<<WGM13);
            break;
      case 9:
            TCCR1A |= (1<<WGM10);
            TCCR1B |= (1<<WGM13);
            break;
      case 10:
            TCCR1A |= (1<<WGM11);
            TCCR1B |= (1<<WGM13);
            break;
      case 11:
            TCCR1A |= (3<<WGM10);
            TCCR1B |= (1<<WGM13);
            break;
      case 12:
            TCCR1B |= (3<<WGM12);
            break;
      case 13:
            TCCR1A |= (1<<WGM10);
            TCCR1B |= (3<<WGM12);
            break;
      case 14:
            TCCR1A |= (1<<WGM11);
            TCCR1B |= (3<<WGM12);
            break;
      case 15:
            TCCR1A |= (3<<WGM10);
            TCCR1B |= (3<<WGM12);
            break;
      default :
            return 1;
    }
return 0;
}

int confModoSalidas_TC(uint8_t outA, uint8_t outB){
/**
* Nota: los pines asociados a la temporización que se deseen utilizar con algún modo de salida, deben ser declarados como salidas por código antes de habilitar el timer
* Función para selección del modo de las salidas del timer
* @param outA: modo salida A
* @param outB: modo salida B
* @returns 0/1 : 0 si todo funciono 1 si algo anda mal
*/
  TCCR1A &=~ (3<<COM1A0);
  TCCR1A &=~ (3<<COM1B0);
  switch (outA) {
    case 0: //OC1A off
          TCCR1A &=~ (3<<COM1A0);
          break;
    case 1: //OC1A toggle
          TCCR1A |= (1<<COM1A0);
          break;
    case 2: //OC1A clear
          TCCR1A |= (1<<COM1A1);
          break;
    case 3: //OC1A set
          TCCR1A |= (3<<COM1A0);
    default :
            return 1;
  }
  switch (outB) {
    case 0: //OC1B off
          TCCR1A &=~ (3<<COM1B0);
          break;
    case 1: //OC1B toggle
          TCCR1A |= (1<<COM1B0);
          break;
    case 2: //OC1B clear
          TCCR1A |= (1<<COM1B1);
          break;
    case 3: //OC1B set
          TCCR1A |= (3<<COM1B0);
    default :
            return 1;
  }
return 0;
}

int interrupciones_TC(uint8_t InputCapt, uint8_t OutputCaptA, uint8_t OutputCaptB, uint8_t Overflow){
/*
* función para la habilitación opcional de las distintas fuentes de interrupción disponibles
* @param InputCapt: activa interrupciones por InputCapt
* @param OutputCaptA: activa interrupción por
* @param OutputCaptB: activa interrupción probar
* @parm Overflow: activa interrupción por Overflow
* @returns 0/1: si todo marcha bien/mal
*/
    if (InputCapt) {
      TIFR1 &=~ (1<<ICF1); //apaga flag
      TIMSK1 |= (1<<ICIE1); //habilita interrupcion
    }
    if (OutputCaptA) {
      TIFR1 &=~ (1<<OCF1A); //apaga flag
      TIMSK1 |= (1<<OCIE1A); //habilita interrupcion
    }
    if (OutputCaptB) {
      TIFR1 &=~ (1<<OCF1B); //apaga flag
      TIMSK1 |= (1<<OCIE1B); //habilita interrupcion
    }
    if (Overflow) {
      TIFR1 &=~ (1<<TOV1); //apaga flag
      TIMSK1 |= (1<<TOIE1); //habilita interrupcion
    }
return 0;
}


void setDutyA(float dutyA){
  /* dutyA en ms
  * funciones  específicas  actualiza el valor del duty cicle A
  */
  OCR1A = (uint16_t)((dutyA/1000)*(F_CPU/prescalerValue_TC)+1); //Numero hasta el cual cuenta para llegar al
                                                               // tiempo del periodo de trabajo A del PWM
}

void setDutyB(float dutyB){
  /* dutyB en ms
  * funciones  específicas  actualiza el valor del duty cicle B
  */
  OCR1B = (uint16_t)((dutyB/1000)*(F_CPU/prescalerValue_TC)+1); //Numero hasta el cual cuenta para llegar al
                                                              // tiempo del periodo de trabajo B del PWM
}


void setTP(float T) {
  /* T en ms
  * funciones  específicas  para PWM 14 que setea el valor del periodo base del pwm
  */

      ICR1 = (uint16_t)((T/1000)*(F_CPU/prescalerValue_TC)+1); //Numero hasta el cual cuenta para llegar al
                                                              // tiempo del periodo base del PWM
}
