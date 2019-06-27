#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
/*Drivers*/
#include "timer_uno.h"
#include "i2c_master.h"
#include "mpu_6050.h"
#include "i2c_DEV.h"
#include "UART_ATmega0.h"
#include "pid.h"
#include "gpio_register_atmega328p.h"
#include "processing_data.h"

#define DEV_ADDR 0x68			// Direccion estandar de MPU
// -------------- TIMERS ---------------
#define TP_CONTROL_LOOP 10   //Periodo en ms para el control del robot
/* TIMER 0*/
#define MODE_TIMER0 1        // Phase Correct PWM Mode 1
#define MODE_OC0A 2          // OUTPUT Compare pin (OC0A) en modo clear 2
#define MODE_OC0B 3          // OUTPUT Compare pin (OC0B) en modo set 3
#define PRESCALER_TIMER0 1
/* TIMER 2*/
#define MODE_TIMER2 1        // Phase Correct PWM Mode 1
#define MODE_OC2A 2          // OUTPUT Compare pin (OC2A) en modo clear 2
#define MODE_OC2B 3          // OUTPUT Compare pin (OC2B) en modo set 3
#define PRESCALER_TIMER2 1
/* TIMER 1*/
#define MODE_TIMER1 4        // CTC Mode 4
#define MODE_OC1A 1          // OUTPUT Compare pin (OC1A) en modo toggle 1
#define MODE_OC1B 0          // OUTPUT Compare pin (OC1B) en modo off 0
#define TIEMPO_TIMER1 2  // Tiempo en ms para definir el prescaler
/*USART declaracion de tipo stream de E/S*/
FILE uart_io = FDEV_SETUP_STREAM(mi_putc0, mi_getc0, _FDEV_SETUP_RW); // Declara un tipo stream de E/S
/*Interrupcion del timer y flag para temporizacion*/
int flag_timer1 = 1;
ISR(TIMER1_COMPA_vect){
    flag_timer1 = 0;
}
/*------------------------------------------------*/
int main(void) {
  sei(); // Habilita interrupciones
//-----------------------------------------//
//-----    CONFIGURACION I2C Y MPU    -----//
//-----------------------------------------//
  i2c_init(100000UL);
	_delay_ms(10);
	DEV_write(0,MPU6050_RA_PWR_MGMT_1, MPU6050_RA_ZA_OFFS_L_TC);		// Activa MPU
	DEV_write(0,MPU6050_RA_CONFIG, MPU6050_DLPF_BW_10);			// filtro LP 10hz (se configura con mpu_6050.h )  DLPF-> Digital low pass filter
	                                                        //DEV_write(0, SMPLRT_DIV, 0x04);
//Configuracion del gyroscopo FS_SEL  "Full scale range" Segun tabla de mapa de registros PAG 14.
	DEV_write(0,MPU6050_RA_GYRO_CONFIG, MPU6050_GYRO_FS_250 );   //+-250°/s
//Configuracion del gyroscopo FS_SEL  "Full scale range" Segun tabla de mapa de registros PAG 14.
	DEV_write(0,MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_2); //+-2g
//-----------------------------------------//
//-----      CONFIGURACION USART      -----//
//-----------------------------------------//
	mi_UART_Init0(9600,0,0);
	stdout = stdin = &uart_io;  // El stream (FILE) uart_io es la E/S estandar, es decir para putc y getc
	printf("OK\r\n");
//-----------------------------------------//
//--------   Timer 8bit 2 para PWM  -------//
//-----------------------------------------//
  D3_salida;
  B3_salida;
  confModo_T8(MODE_TIMER2,2);
  confModoSalidas_T8(MODE_OC2A, MODE_OC2B,2);
  interrupciones_T8(0,0,0,2);
  confPrescaler_T8(PRESCALER_TIMER2,2);
  // setDutyA2(100);
  // setDutyB2(0);
//-----------------------------------------//
//-------- Timer 8bit 0 para PWM    -------//
//-----------------------------------------//
  D6_salida;
  D5_salida;
  confModo_T8(MODE_TIMER0,0);
  confModoSalidas_T8(MODE_OC0A, MODE_OC0B,0);
  interrupciones_T8(0,0,0,0);
  confPrescaler_T8(PRESCALER_TIMER0,0);
  // setDutyA0(100);
  // setDutyB0(0);
//-----------------------------------------//
//---- Timer 16bit para temporizacion  ----//
//-----------------------------------------//
  confModo_T16(MODE_TIMER1);
  confPrescaler_T16(TIEMPO_TIMER1);
  confModoSalidas_T16(MODE_OC1A, MODE_OC1B);
  interrupciones_T16(0, 1, 0, 0);  //interrupcion por compare match con OC1A
  setDutyA16(TIEMPO_TIMER1);
//-----------------------------------------//
//---------------   PID    ----------------//
//-----------------------------------------//
// double u = 0; //Accion de control (salida del pid)
	setSamplingTime(TIEMPO_TIMER1); // 10 ms
	setControllerGains(7.6, 0.0,0.0);  //kp -- ki -- kd

/*Variables para el control*/
  double AnguloPID;
  double error;
  double outPID;
  uint8_t OCRnX;
/*Control loop*/
  while (1) {
    while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer
    /*PID*/
    AnguloPID = getAngulo();
    error = AnguloPID - SETPOINT;
    outPID = pid(error);
    if (outPID > 204) outPID = 204;
    else if(outPID < 51) outPID = 51;
    OCRnX = (uint8_t) outPID;
    setDutyA0(OCRnX);
    setDutyB0(OCRnX);
    setDutyA2(OCRnX);
    setDutyB2(OCRnX);
    flag_timer1 = 1; // Bandera de temporizacion
  }
  return 0;
}
