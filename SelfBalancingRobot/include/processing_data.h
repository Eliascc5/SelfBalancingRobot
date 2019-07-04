
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


//-------------- MPU6050 ----------------
#define GYROSENSITIVITY 131.0	// = 131 LSB/degrees/sec Pag 12 datasheet
#define ACCELSENSITIVITY 16384.0 // = 16384 LSB/g         Pag 12 datasheet
#define RAD_TO_DEG  180/3.141592654  //Para conversion
#define PI 3.1416
#define PERIOD_MPU 0.00001   //Frecuencia de operacion 100kHz
#define DT 0.005
#define ALPHA 0.1


double getAngulo(){

int16_t ax_raw=0, ay_raw=0, az_raw=0,gx_raw=0, gy_raw=0, gz_raw=0;

double ax_angle ,ay_angle, gx_angle, gy_angle;
//double anguloX=0;
double anguloY=90;


//Contiene los 8 bits mas signiticativos por eso el ACCEL_XOUT_H  (high)
//La aceleracion en verdad esta compuesta por 16 bits
ax_raw = DEV_read16(0, MPU6050_RA_ACCEL_XOUT_H);//ACCELSENSITIVITY;// | DEV_read(0, MPU6050_RA_ACCEL_XOUT_L))/ACCELSENSITIVITY;

// ay_raw = DEV_read16(0, MPU6050_RA_ACCEL_YOUT_H);//ACCELSENSITIVITY; //| DEV_read(0, MPU6050_RA_ACCEL_YOUT_L))/ACCELSENSITIVITY;

az_raw = DEV_read16(0, MPU6050_RA_ACCEL_ZOUT_H);//ACCELSENSITIVITY;//| DEV_read(0, MPU6050_RA_ACCEL_ZOUT_L))/ACCELSENSITIVITY;

// gx_raw = DEV_read16(0, MPU6050_RA_GYRO_XOUT_H); //| DEV_read(0,MPU6050_RA_GYRO_XOUT_L));

gy_raw = DEV_read16(0,MPU6050_RA_GYRO_YOUT_H) ;//| DEV_read(0,MPU6050_RA_GYRO_YOUT_L));

// gz_raw = DEV_read16(0, MPU6050_RA_GYRO_ZOUT_H);// | DEV_read(0,MPU6050_RA_GYRO_ZOUT_L));

//-------------- Processing data ------------------

//gx_raw = (gx_raw/GYROSENSITIVITY);

gy_raw = (gy_raw/GYROSENSITIVITY);

//gx_angle = (double) ((double)gx_raw * PERIOD_MPU);  //Para obtener lectura en grados
gy_angle = (double) ((double)gy_raw * DT);	//Para obtener lectura en grados

ay_angle = ((atan2 (ax_raw, az_raw) ) * RAD_TO_DEG); //

//ax_angle = ((atan2 (ay_raw, az_raw) + 3/4*PI ) * RAD_TO_DEG);

// FILTRO COMPLEMENTARIO

//anguloX = 0.5 * (anguloX + gx_angle) + 0.5 * ax_angle;

anguloY = ALPHA * (anguloY + gy_angle) + (1-ALPHA) * ay_angle;

//Casting to print

//int AX= (int)(anguloX * 1);
//int AY= (int)(gy_angle * 100000);

//int Angulo_PID= (int)(anguloY * 1);

//printf("%d\n",Angulo_PID);
//_delay_ms(10);



return anguloY;
}
