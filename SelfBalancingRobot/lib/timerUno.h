#ifndef timerUno_h
#define timerUno_h

#include <stdio.h>

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


#endif
