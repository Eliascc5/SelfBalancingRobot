/*
i2c con MPU6050
 */
//#define F_CPU 16000000
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include "i2c_master.h"
#define DEV_ADDR 0x68			// Direccion estandar de MPU
#include "mpu_6050.h"
#include "i2c_DEV.h"
#include "UART_ATmega0.h"
#include <util/delay.h>
#include <math.h>

FILE uart_io = FDEV_SETUP_STREAM(mi_putc0, mi_getc0, _FDEV_SETUP_RW); // Declara un tipo stream de E/S

int main(void){

	//Variables que declaro para almacenar las lecturas del sensor
	int16_t x_acel, y_acel, z_acel;
	int16_t x_gyro, y_gyro, z_gyro;


	//Facores de sensibilidad
	uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec Pag 12 datasheet
	uint16_t  accelsensitivity = 16384; // = 16384 LSB/g         Pag 12 datasheet



	i2c_init(100000UL);
	_delay_ms(10);

/*
 //------TIMER for main loop timing--------------
	//with prescale 8
	//At 8MHz 0.01s = 10 000 unit
	TCCR1A=0X00;
	TCCR1B |=1<<CS11;
*/


	DEV_write(0,MPU6050_RA_PWR_MGMT_1, 0x0B);		// Activa MPU
	DEV_write(0,MPU6050_RA_CONFIG, 0x05);			// filtro LP 10hz

//Configuracion del gyroscopo FS_SEL  "Full scale range" Segun tabla de mapa de registros PAG 14.
	DEV_write(0,MPU6050_RA_GYRO_CONFIG, MPU6050_GYRO_FS_250 );   //+-250°/s

//Configuracion del gyroscopo FS_SEL  "Full scale range" Segun tabla de mapa de registros PAG 14.
	DEV_write(0,MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_2); //+-2g


	mi_UART_Init0(9600,0,0);
	stdout = stdin = &uart_io;  // El stream (FILE) uart_io es la E/S estandar, es decir para putc y getc
printf("OK\r\n");

	_delay_ms(3000);


    while (1) {
			//Contiene los 8 bits mas signiticativos por eso el ACCEL_XOUT_H  (high)
			//La aceleracion en verdad esta compuesta por 16 bits


			//Sensitivity scale factor: 16384

				x_acel = DEV_read16(0, MPU6050_RA_ACCEL_XOUT_H) << 8 |
				DEV_read16(0, MPU6050_RA_ACCEL_XOUT_L);

				y_acel = DEV_read16(0, MPU6050_RA_ACCEL_YOUT_H) << 8 |
				DEV_read16(0, MPU6050_RA_ACCEL_YOUT_L);

				z_acel = DEV_read16(0, MPU6050_RA_ACCEL_ZOUT_H) << 8 |
				DEV_read16(0, MPU6050_RA_ACCEL_ZOUT_L);

				x_gyro = DEV_read16(0, MPU6050_RA_GYRO_XOUT_H) << 8 |
				DEV_read16(0,MPU6050_RA_GYRO_XOUT_L);

				y_gyro = DEV_read16(0,MPU6050_RA_GYRO_YOUT_H) << 8 |
				DEV_read16(0,MPU6050_RA_GYRO_YOUT_L);

				z_gyro = DEV_read16(0, MPU6050_RA_GYRO_ZOUT_H) << 8 |
				DEV_read16(0,MPU6050_RA_GYRO_ZOUT_L);


printf("A: %d, %d, %d\n", (int)x_acel/accelsensitivity, (int)y_acel/accelsensitivity, (int)z_acel/accelsensitivity);

printf("G: %d, %d, %d\n\n", (int)x_gyro/gyrosensitivity, (int)y_gyro/gyrosensitivity, (int)z_gyro/gyrosensitivity);


    }
}

// #include "timer_uno.h"
// #include <util/delay.h>
// #include <time.h>
// #include <stdlib.h>
// #include <avr/interrupt.h>
// #include <avr/io.h>
// #include <stdio.h>
//
// int flag_timer0 = 0;
//
// ISR(TIMER0_COMPA_vect){
//   // _delay_ms(100);
//   flag_timer0 = 1;
//   PORTD ^= (1<<PORTD4);
//   TIFR0 &=~ (1<<OCF0A); //apaga flag
// }
// int main(int argc, char const *argv[]) {
//   DDRD |= (1<<PORTD6) | (1<<PORTD4);
//   // PORTD |= (1<<PORTD4);
//   confModo_T8(2);
//   confModoSalidas_T8(1, 0);
//   interrupciones_T8(1,0,0);
//   confPrescaler_T8(10);
//   // // conf_PWM_14_TC1(1000, 100, 900);
//   setDutyA8(10);
//   sei();
//   // setDutyB8(900);
//   // TCCR0A |= (1<<WGM01);
//   // TCCR0A |= (1<<COM0A0);
//   // TCCR0B |= (5<<CS00);
//   // OCR0A = (uint8_t)(255); //Numero hasta el cual cuenta para llegar al
//   // sOLO PUEDO CONTAR HASTA 256 -> LAS FRECUENCIAS SON MAS CHICAS.
//   while (1) {
//       while(flag_timer0 == 0){_delay_us(1);}
//
//   }
//
//   return 0;
// }
