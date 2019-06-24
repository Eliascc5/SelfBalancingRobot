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
#include <avr/interrupt.h>
#include <math.h>




#define GYROSENSITIVITY 131.0	// = 131 LSB/degrees/sec Pag 12 datasheet
#define ACCELSENSITIVITY 16384.0 // = 16384 LSB/g         Pag 12 datasheet
#define RAD_TO_DEG  180/3.141592654  //Para conversion
#define PI 3.1416

//-------------HEADERS------------
float getOffset_gyroX(int16_t gx_raw_, int16_t gy_raw_, int16_t gz_raw_);
float getOffset_ax(int16_t ax_,int16_t ay_,int16_t az_);
float getOffset_ay(int16_t ax_,int16_t ay_,int16_t az_);
//-----------------



//Variable Globales para el control del tiempo
volatile unsigned long timer1_millis=0;
unsigned long timePrev=0, timeAct=0,dt=0;        //Variables for time control

ISR (TIMER1_COMPA_vect)
{
	timer1_millis++;
}

//-------------------------------------------------------------------------------------




FILE uart_io = FDEV_SETUP_STREAM(mi_putc0, mi_getc0, _FDEV_SETUP_RW); // Declara un tipo stream de E/S


unsigned long millis(){
	unsigned long m;
	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer1_millis;
	return m;
}

float getOffset_ay(int16_t ax_,int16_t ay_,int16_t az_){

	float ay_angle_error;
	int acc_error=0;


	if(acc_error==0){
		for(int a=0; a<200; a++){

			ax_ = (DEV_read16(0, MPU6050_RA_ACCEL_XOUT_H))/ACCELSENSITIVITY;

			ay_ = (DEV_read16(0, MPU6050_RA_ACCEL_YOUT_H))/ACCELSENSITIVITY;

			az_ = (DEV_read16(0, MPU6050_RA_ACCEL_ZOUT_H))/ACCELSENSITIVITY;

			ay_angle_error = (atan2 (ax_, az_) + PI) * RAD_TO_DEG;

			if(a==199){
				ay_angle_error = ay_angle_error/200;
				acc_error=1;
			}
		}
	}
	return ay_angle_error;
}

float getOffset_ax(int16_t ax_,int16_t ay_,int16_t az_){

	float ax_angle_error;
	int acc_error=0;


	if(acc_error==0){
		for(int a=0; a<200; a++){

			ax_ = (DEV_read16(0, MPU6050_RA_ACCEL_XOUT_H))/ACCELSENSITIVITY;

			ay_ = (DEV_read16(0, MPU6050_RA_ACCEL_YOUT_H))/ACCELSENSITIVITY;

			az_ = (DEV_read16(0, MPU6050_RA_ACCEL_ZOUT_H))/ACCELSENSITIVITY;

			ax_angle_error = (atan2 (ay_, az_) + PI) * RAD_TO_DEG;

			if(a==199){
				ax_angle_error = ax_angle_error/200;
				acc_error=1;
			}
		}
	}
	return ax_angle_error;
}

float getOffset_gyroX(int16_t gx_raw_, int16_t gy_raw_, int16_t gz_raw_){

	float gx_raw_error;
	int gyro_error=0;


  if(gyro_error==0){
    for(int i=0; i<200; i++){

			gx_raw_ = (DEV_read16(0, MPU6050_RA_GYRO_XOUT_H));

   		gx_raw_error = gx_raw_error + (float)(gx_raw_/GYROSENSITIVITY);

      if(i==199){
        gx_raw_error = gx_raw_error/200;
        gyro_error=1;
      }
    }
  }
	return gx_raw_error;
}

float getOffset_gyroY(int16_t gx_raw_, int16_t gy_raw_, int16_t gz_raw_){

	float gy_raw_error;
	int gyro_error=0;


  if(gyro_error==0){
    for(int i=0; i<200; i++){

			gy_raw_ = (DEV_read16(0, MPU6050_RA_GYRO_XOUT_H));

   		gy_raw_error = gy_raw_error + (float)(gy_raw_/GYROSENSITIVITY);

      if(i==199){
        gy_raw_error = gy_raw_error/200;
        gyro_error=1;
      }
    }
  }

	return gy_raw_error;
}



int main(void){

	//Variables donde se almacenan las lecturas crudas del sensor

	int16_t ax_raw=0, ay_raw=0, az_raw=0,gx_raw=0, gy_raw=0, gz_raw=0;

	float offset_acel_y , offset_acel_x, offset_gyro_x, offset_gyro_y;

	float ax_angle ,ay_angle, gx_angle, gy_angle;

	float anguloX=0, anguloY=0;

	//FAST MODE = 400400KHz
	//Standard mode =100kHz
	i2c_init(100000UL);
	_delay_ms(10);

	DEV_write(0,MPU6050_RA_PWR_MGMT_1, 0x0B);		// Activa MPU
	DEV_write(0,MPU6050_RA_CONFIG, MPU6050_DLPF_BW_10);			// filtro LP 10hz (se configura con mpu_6050.h )  DLPF-> Digital low pass filter
	//DEV_write(0, SMPLRT_DIV, 0x04);

//Configuracion del gyroscopo FS_SEL  "Full scale range" Segun tabla de mapa de registros PAG 14.
	DEV_write(0,MPU6050_RA_GYRO_CONFIG, MPU6050_GYRO_FS_250 );   //+-250°/s
//Configuracion del gyroscopo FS_SEL  "Full scale range" Segun tabla de mapa de registros PAG 14.
	DEV_write(0,MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_2); //+-2g

	mi_UART_Init0(57600,0,0);
	stdout = stdin = &uart_io;  // El stream (FILE) uart_io es la E/S estandar, es decir para putc y getc
printf("OK\r\n");

	printf("a");
	offset_acel_y = getOffset_ay( ax_raw, ay_raw,  az_raw);
	offset_acel_x = getOffset_ax( ax_raw, ay_raw,  az_raw);
	offset_gyro_x = getOffset_gyroX(gx_raw, gy_raw,  gz_raw);
	offset_gyro_y = getOffset_gyroY(gx_raw, gy_raw,  gz_raw);

	while (1) {

		timePrev = millis();
		//Contiene los 8 bits mas signiticativos por eso el ACCEL_XOUT_H  (high)
		//La aceleracion en verdad esta compuesta por 16 bits
		ax_raw = DEV_read16(0, MPU6050_RA_ACCEL_XOUT_H)/ACCELSENSITIVITY;// | DEV_read(0, MPU6050_RA_ACCEL_XOUT_L))/ACCELSENSITIVITY;

		ay_raw = DEV_read16(0, MPU6050_RA_ACCEL_YOUT_H)/ACCELSENSITIVITY; //| DEV_read(0, MPU6050_RA_ACCEL_YOUT_L))/ACCELSENSITIVITY;

		az_raw = DEV_read16(0, MPU6050_RA_ACCEL_ZOUT_H)/ACCELSENSITIVITY;//| DEV_read(0, MPU6050_RA_ACCEL_ZOUT_L))/ACCELSENSITIVITY;

		gx_raw = DEV_read16(0, MPU6050_RA_GYRO_XOUT_H); //| DEV_read(0,MPU6050_RA_GYRO_XOUT_L));

		gy_raw = DEV_read16(0,MPU6050_RA_GYRO_YOUT_H) ;//| DEV_read(0,MPU6050_RA_GYRO_YOUT_L));

		gz_raw = DEV_read16(0, MPU6050_RA_GYRO_ZOUT_H);// | DEV_read(0,MPU6050_RA_GYRO_ZOUT_L));

		//--------------------------------

		gx_raw = (gx_raw/GYROSENSITIVITY) - offset_gyro_x;

		gy_raw = (gy_raw/GYROSENSITIVITY) - offset_gyro_y;


		timeAct=millis();
		dt = (timeAct - timePrev) / 1000.0;

		gx_angle = gx_raw * (float)dt;

		gy_angle = gy_raw * (float)dt;

<<<<<<< HEAD
		ay_angle = ((atan2 (ax_raw, az_raw) + PI) * RAD_TO_DEG) - offset_acel_y;

		ax_angle = ((atan2 (ay_raw, az_raw) + PI) * RAD_TO_DEG) - offset_acel_x;


		// FILTRO COMPLEMENTARIO

		anguloX = 0.98 * (anguloX + gx_angle) + 0.02 * ax_angle;

		anguloY = 0.98 * (anguloY + gy_angle) + 0.02 * ay_angle;

		//Casting to print

		int AX,AY;

		AX= (int)anguloX * 1;
		AY= (int)anguloY * 1;

		printf("%d,%d\n",AX,AY);
		_delay_ms(1000);




		}


	}
=======
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
>>>>>>> 67ad38b0ab72c57aa4d3cb81e48c206872a936d0
