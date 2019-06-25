#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include <util/delay.h>
#include <avr/interrupt.h>

#include <avr/io.h>

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
#define MODE_TIMER0 3        // Modo Fast PWM timer 0
#define MODE_OC0A 2          // OUTPUT Compare pin (OC0A) en modo toggle 1
#define MODE_OC0B 2          // OUTPUT Compare pin (OC0B) en modo toggle 1
// volatile unsigned long timer0_millis = 0;


FILE uart_io = FDEV_SETUP_STREAM(mi_putc0, mi_getc0, _FDEV_SETUP_RW); // Declara un tipo stream de E/S
/*Interrupcion del timer y flag para temporizacion*/
// int flag_timer0 = 0;
// ISR(TIMER0_COMPA_vect){
//     flag_timer0 = 1;
// }
/*------------------------------------------------*/

int main(void) {

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
//----- Timer 8bit para temporizacion -----//
//-----------------------------------------//
  // D5_salida;
  // D6_apagado;
  // confModo_T8(3);
  // confModoSalidas_T8(MODE_OC0A, 2);
  // interrupciones_T8(0,0,0);
  // confPrescaler_T8(10);
  // setDutyA8(1000);
  // setDutyB8(10);
//-----------------------------------------//
//-----     Timer 16bit para PWM      -----//
//-----------------------------------------//


  // double u = 0; //Accion de control (salida del pid)

  while (1) {
    //if (flag_timer0){

      double AnguloPID = getAngulo();

			double error = AnguloPID - SETPOINT;

      double outPID = pid(error);

			//Casting to print 
			int mistake =(int)error;
			printf("%d\n",mistake);

      //flag_timer0 = 0;
    //}
  }


  return 0;
}
