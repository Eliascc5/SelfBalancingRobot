#ifndef TIMERUNO_H
#define TIMERUNO_H

#include "gpio_register_atmega328p.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>


#define OCRNX_MAX 250
#define OCRNX_MIN 5
#define OCRNX_GIRO 20       //Para hacer girar el robot
#define OCRNX_OFF 127
/* TIMER 0*/
#define MODE_TIMER0 1        // Phase Correct PWM Mode 1
#define MODE_OC0A 2          // OUTPUT Compare pin (OC0A) en modo clear 2
#define MODE_OC0B 3          // OUTPUT Compare pin (OC0B) en modo set 3
#define PRESCALER_TIMER0 1   // esto nos da un periodo de 0.032ms = (2^8/16Mhz) * 2
/* TIMER 2*/
#define MODE_TIMER2 1        // Phase Correct PWM Mode 1
#define MODE_OC2A 2          // OUTPUT Compare pin (OC2A) en modo clear 2
#define MODE_OC2B 3          // OUTPUT Compare pin (OC2B) en modo set 3
#define PRESCALER_TIMER2 1   // esto nos da un periodo de 0.032ms = (2^8/16Mhz) * 2
/* TIMER 1*/
#define MODE_TIMER1 4        // CTC Mode 4
#define MODE_OC1A 0          // OUTPUT Compare pin (OC1A) en modo (toggle 1 - 0 off)
#define MODE_OC1B 0          // OUTPUT Compare pin (OC1B) en modo off 0
#define TIEMPO_TIMER1 5      // Tiempo en ms para definir el prescaler


uint16_t prescalerValue_T16 = 0;
uint16_t prescalerValue_T8 = 0;

/* Configuracion para timer 16 bits*/
int confPrescaler_T16(float T);
int confModo_T16(uint8_t modo);
int confModoSalidas_T16(uint8_t outA, uint8_t outB);
int interrupciones_T16(uint8_t InputCapt, uint8_t OutputCaptA, uint8_t OutputCaptB, uint8_t Overflow);
void setDutyA16(float dutyA);
void setDutyB16(float dutyB);
void setTP16(float T);

/* Configuracion para timer 8 bits*/
int confPrescaler_T8(uint16_t Pres, int n);
int confModo_T8(uint8_t modo, int n);
int confModoSalidas_T8(uint8_t outA, uint8_t outB, int n);
int interrupciones_T8(uint8_t OutputCaptA, uint8_t OutputCaptB, uint8_t Overflow, int n);
void setDutyA0(uint8_t dutyA);
void setDutyB0(uint8_t dutyB);
void setDutyA2(uint8_t dutyA);
void setDutyB2(uint8_t dutyB);


void Timer_init(void);


#endif
