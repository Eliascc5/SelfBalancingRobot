/* Proyecto Final Microcontroladores y Electronica de Potencia
* Se plantea la fabricacion y control de un robot auto balanceado. Aquise encuentran
* los drivers del robot
* Notas:
*       - Se planteo un control PID para estabilizar el robot.
*       - Para hacer girar al robot se dan consignas complementarias a los motores
*         (Ej: una salida del PID = a 100 se convierte en Motor_I = 80 y Motor_D = 120)
*       - Para hacer avanzar/retroceder al robot se le da una consigna distinta de la de equilibrio (90°)
*         y se espera que el robot semueva para mantener el nuevo setPoint.
*       - Cabe destacar que aun no se ha logrado sintonizar el PID por lo tanto
*         las consideraciones de movimiento antes mencionadas sonhipoteticas
* Lenguaje: C
* Materiales: ATMEGA328p, MPU6050, 2 DC Motors, L298N
* Autores: Coreas Elias y Pino Jeremias
*/

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
#include <util/atomic.h>
/*USART*/
#define UART_BAUDRATE 57600
#define DEV_ADDR 0x68			// Direccion estandar de MPU
// -------------- TIMERS ---------------
#define TP_CONTROL_LOOP 5   //Periodo en ms para el control del robot
#define OCRNX_MAX 204
#define OCRNX_MIN 51
#define OCRNX_GIRO 20       //Para hacer girar el robot
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
#define MODE_OC1A 1          // OUTPUT Compare pin (OC1A) en modo toggle 1
#define MODE_OC1B 0          // OUTPUT Compare pin (OC1B) en modo off 0
#define TIEMPO_TIMER1 5  // Tiempo en ms para definir el prescaler
/*CONTROL*/
#define STATIC_SETPOINT 90.0 		//Para mantenerlo parado. PARA LA ORIENTACION en la que ubicamos el sensor
volatile float SETPOINT = 90.0; //Para moverlo.
      // #define FORWARD_SETPOINT 95.0; //Para moverlo hacia adelante.
      // #define BACKWARD_SETPOINT 85.0; //Para moverlo hacia atras.

/*Estados posibles de la maquina de estados*/
enum tEstados_m{Start, Move_forward, Move_backward, Turn_right, Turn_left, Retain_immobile, Stop}estado_s;

/*USART declaracion de tipo stream de E/S y buffer de interpretar comando*/
FILE uart_io = FDEV_SETUP_STREAM(mi_putc0, mi_getc0, _FDEV_SETUP_RW); // Declara un tipo stream de E/S
unsigned int indcom; // índice para llenar el buffer de recepción
char comando[120]; // buffer de recepción

/*Interrupcion del timer y flag para temporizacion*/
int flag_timer1 = 1;
ISR(TIMER1_COMPA_vect){
// Asegura que no se puede interrumpir
    ATOMIC_BLOCK(ATOMIC_FORCEON){
    flag_timer1 = 0;
    }
    TIFR1 &=~ (1<<OCF1A); //apaga flag
}

void InterpretaComando(void){
	int aux_i;
  double aux_f;
  // printf("Interpreto comando %c\n",comando[0]);
	switch(comando[0]){
    case 'S': //SETPOINT
          aux_i = atoi(&comando[1]); // lee desde el caracter 1 hasta el caracter nulo y lo convierte en int
          if (aux_i >= 70 && aux_i <= 110){
              SETPOINT = aux_i;
          }else{
          printf("ST muy grande:%d\n",SETPOINT);
          }
          break;
		case 'P':
		   aux_f = atof(&comando[1]); // string to float
       setKP(aux_f);
       // aux_f = aux_f * 100;
       // printf("KP  %d\n",(int)aux_f);
		   break;
    case 'I':
		   aux_f = atof(&comando[1]); // string to float
       setKI(aux_f);
       // aux_f = aux_f * 100;
       // printf("KI %d\n",(int)aux_f);
		   break;
    case 'D':
 			 aux_f = atof(&comando[1]); // string to float
       setKD(aux_f);
       // aux_f = aux_f * 100;
       // printf("KD %d\n",(int)aux_f);
 			 break;
    case 'F':
      estado_s = Move_forward;
      break;
    case 'B':
      estado_s = Move_backward;
      break;
    case 'R':
      estado_s = Turn_right;
      break;
    case 'L':
      estado_s = Turn_left;
      break;
    case 'A':
      estado_s = Retain_immobile;
      break;
    case 'O':
      estado_s = Stop;
      break;
    case 'E': //Consulta estado. Para hacerlo mas detallado se podria hacer switch estado y printear el nombre en lugar del numero
      printf("Estado %d\n", estado_s);
      break;
    default:
      break;
  }
}

/*Interrupcion recepcion uart*/
ISR(USART_RX_vect){
  char dato;
  dato=getc();
  switch(dato){
    case ':': // Delimitador de inicio
      indcom=0; // Inicializa índice de buffer de recepción
      break;
    case 13: // Delimitador de final, retorno de carro
      comando[indcom]=0; // coloca \0 (caracter nulo) luego del último caracter recibido antes del CR.
                         //Esto es util para usar atoi o atof que leen hasta el caracter nulo
      InterpretaComando();// Llama a función intérprete de comandos
      break;
    default: // Todo lo que está entre delimitadores,
      comando[indcom++]=dato; // Guarda en elemento del buffer e incrementa indcom para apuntar a siguiente
      break;
  }
  UCSR0A &=~ (1<<RXC0) ;  //Apago el flag de recepcion completa
}

/*Rutina para inicializar los timers*/
void Timer_init(void){
  //-----------------------------------------//
  //--------   Timer 8bit 2 para PWM  -------//
  //-----------------------------------------//
    D3_salida; B3_salida;
    confModo_T8(MODE_TIMER2,2);
    confModoSalidas_T8(MODE_OC2A, MODE_OC2B,2);
    interrupciones_T8(0,0,0,2);
    confPrescaler_T8(PRESCALER_TIMER2,2);
  //-----------------------------------------//
  //-------- Timer 8bit 0 para PWM    -------//
  //-----------------------------------------//
    D6_salida; D5_salida;
    confModo_T8(MODE_TIMER0,0);
    confModoSalidas_T8(MODE_OC0A, MODE_OC0B,0);
    interrupciones_T8(0,0,0,0);
    confPrescaler_T8(PRESCALER_TIMER0,0);
  //-----------------------------------------//
  //---- Timer 16bit para temporizacion  ----//
  //-----------------------------------------//
    confModo_T16(MODE_TIMER1);
    confPrescaler_T16(TIEMPO_TIMER1);
    confModoSalidas_T16(MODE_OC1A, MODE_OC1B);
    interrupciones_T16(0, 1, 0, 0);  //interrupcion por compare match con OC1A
    setDutyA16(TIEMPO_TIMER1);
}

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
	DEV_write(0,MPU6050_RA_GYRO_CONFIG, MPU6050_GYRO_FS_250);   //+-250°/s
//Configuracion del gyroscopo FS_SEL  "Full scale range" Segun tabla de mapa de registros PAG 14.
	DEV_write(0,MPU6050_RA_ACCEL_CONFIG, MPU6050_ACCEL_FS_2); //+-2g

//-----------------------------------------//
//-----      CONFIGURACION USART      -----//
//-----------------------------------------//
	mi_UART_Init0(UART_BAUDRATE,0,1);
  stdout = stdin = &uart_io;  // El stream (FILE) uart_io es la E/S estandar, es decir para putc y getc
  printf("OK\n");

/*Habilita interrupciones*/
  sei();

/*Inicializacion de Timers*/
  Timer_init();

//-----------------------------------------//
//---------------   PID    ----------------//
//-----------------------------------------//
// double u = 0; //Accion de control (salida del pid)
	setSamplingTime(TP_CONTROL_LOOP); // 10 ms
	setControllerGains(6.6, 0.0, 0.037);  //kp -- ki -- kd 0632

/*Variables para el control*/
  double AnguloPID;
  double error;
  double outPID;
  uint8_t OCRnX;

/*ESTADO INICIAL*/
estado_s = Start;

/*Control loop*/
  while (1) {
    switch (estado_s) {
      case Start:
          /*MOTOR IZQUIERDO*/
          setDutyA0(127);
          setDutyB0(127);
          /*MOTOR DERECHO */
          setDutyA2(127);
          setDutyB2(127);
          _delay_ms(50);
          break;
      case Move_forward:
          while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer
          SETPOINT = 85.0; //Esto hay que sacarlo de aca y setearlo en el interpreta comando
          /*PID*/
          AnguloPID = getAngulo();
          error =  SETPOINT - AnguloPID;
          outPID = pid(error);

          /*Saturacion del PWM*/
          if (outPID > OCRNX_MAX) {outPID = OCRNX_MAX;}
          if(outPID < OCRNX_MIN) {outPID = OCRNX_MIN;}

          OCRnX = (uint8_t) outPID;

          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX);
          setDutyB0(OCRnX);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX);
          setDutyB2(OCRnX);
          break;
      case Move_backward:
          while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer
          SETPOINT = 95.0; //Esto hay que sacarlo de aca y setearlo en el interpreta comando
          /*PID*/
          AnguloPID = getAngulo();
          error =  SETPOINT - AnguloPID;
          outPID = pid(error);

          /*Saturacion del PWM*/
          if (outPID > OCRNX_MAX) {outPID = OCRNX_MAX;}
          if(outPID < OCRNX_MIN) {outPID = OCRNX_MIN;}

          OCRnX = (uint8_t) outPID;

          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX);
          setDutyB0(OCRnX);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX);
          setDutyB2(OCRnX);
          break;
      case Turn_right:
          while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer
          /*PID*/
          AnguloPID = getAngulo();
          error =  SETPOINT - AnguloPID;
          outPID = pid(error);

          /*Saturacion del PWM*/
          if (outPID > OCRNX_MAX) {outPID = OCRNX_MAX;}
          if(outPID < OCRNX_MIN) {outPID = OCRNX_MIN;}

          OCRnX = (uint8_t) outPID;

          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX - OCRNX_GIRO);
          setDutyB0(OCRnX - OCRNX_GIRO);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX + OCRNX_GIRO);
          setDutyB2(OCRnX + OCRNX_GIRO);

          break;
      case Turn_left:
          while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer
          /*PID*/
          AnguloPID = getAngulo();
          error =  SETPOINT - AnguloPID;
          outPID = pid(error);

          /*Saturacion del PWM*/
          if (outPID > OCRNX_MAX) {outPID = OCRNX_MAX;}
          if(outPID < OCRNX_MIN) {outPID = OCRNX_MIN;}

          OCRnX = (uint8_t) outPID;

          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX + OCRNX_GIRO);
          setDutyB0(OCRnX + OCRNX_GIRO);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX - OCRNX_GIRO);
          setDutyB2(OCRnX - OCRNX_GIRO);
          break;
      case Retain_immobile:
          // printf("Retain inmmobile\n" );
          while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer
          /*PID*/
          AnguloPID = getAngulo();
          error =  STATIC_SETPOINT - AnguloPID;
          outPID = pid(error);

          /*Saturacion del PWM*/
          if (outPID > OCRNX_MAX) {outPID = OCRNX_MAX;}
          if(outPID < OCRNX_MIN) {outPID = OCRNX_MIN;}

          OCRnX = (uint8_t) outPID;
          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX);
          setDutyB0(OCRnX);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX);
          setDutyB2(OCRnX);
          break;
      case Stop:
          return 0;
      default:
          break;
    }
    // printf("Estado %d\n", estado_s);
    flag_timer1 = 1; // Bandera de temporizacion

  }
  return 0;
}
