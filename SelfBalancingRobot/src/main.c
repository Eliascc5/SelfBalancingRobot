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
* Autores: Correa Elias y Pino Jeremias
*/

#include"main.h"


/*USART*/
#define UART_BAUDRATE 57600
#define DEV_ADDR 0x68			// Direccion estandar de MPU
// -------------- TIMERS ---------------
#define TP_CONTROL_LOOP 5   //Periodo en ms para el control del robot
                            // si se cambia hay que cambiarlo en processing_data

/*CONTROL*/
#define STATIC_SETPOINT 90.0 		//Para mantenerlo parado. PARA LA ORIENTACION en la que ubicamos el sensor
volatile float SETPOINT = 90.0; //Para moverlo.

      // #define FORWARD_SETPOINT 95.0; //Para moverlo hacia adelante.
      // #define BACKWARD_SETPOINT 85.0; //Para moverlo hacia atras.

/*USART declaracion de tipo stream de E/S y buffer de interpretar comando*/
FILE uart_io = FDEV_SETUP_STREAM(mi_putc0, mi_getc0, _FDEV_SETUP_RW); // Declara un tipo stream de E/S
unsigned int indcom; // índice para llenar el buffer de recepción
char comando[30]; // buffer de recepción

/*Interrupcion del timer y flag para temporizacion*/
int flag_timer1 = 1;

/*------------------------------------------------------------------------------*/
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
	setSamplingTime(TP_CONTROL_LOOP); // 5 ms
	setControllerGains(10, 0.0, 0.01);  //kp -- ki -- kd 0632
/*Variables para el control*/
double AnguloPID;
double error;
double outPID; //Accion de control (salida del pid)
uint8_t OCRnX; //variable auxiliar para setear los pwm

/*ESTADO INICIAL*/
estado_s = Retain_immobile;

/*Control loop*/
  while (1) {
    switch (estado_s) {
      case Start:
          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRNX_OFF);
          setDutyB0(OCRNX_OFF);
          /*MOTOR DERECHO */
          setDutyA2(OCRNX_OFF);
          setDutyB2(OCRNX_OFF);
          _delay_ms(1);
          break;
      case Move_forward:
          while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer

          /*PID*/
          AnguloPID = getAngulo();
          error =  SETPOINT - AnguloPID;
          outPID = pid(error);

          /*Saturacion del PWM*/
          if (outPID > OCRNX_MAX) {outPID = OCRNX_MAX;}
          if(outPID < OCRNX_MIN) {outPID = OCRNX_MIN;}
          if(AnguloPID > ANG_MAX || AnguloPID < ANG_MIN ) outPID = OCRNX_OFF;

          OCRnX = (uint8_t) outPID;

          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX);
          setDutyB0(OCRnX);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX);
          setDutyB2(OCRnX);

          flag_timer1 = 1; // Bandera de temporizacion
          break;
      case Move_backward:
          while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer

          /*PID*/
          AnguloPID = getAngulo();
          error =  SETPOINT - AnguloPID;
          outPID = pid(error);

          /*Saturacion del PWM*/
          if (outPID > OCRNX_MAX) {outPID = OCRNX_MAX;}
          if(outPID < OCRNX_MIN) {outPID = OCRNX_MIN;}
          if(AnguloPID > ANG_MAX || AnguloPID < ANG_MIN ) outPID = OCRNX_OFF;

          OCRnX = (uint8_t) outPID;

          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX);
          setDutyB0(OCRnX);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX);
          setDutyB2(OCRnX);

          flag_timer1 = 1; // Bandera de temporizacion
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
          if(AnguloPID > ANG_MAX || AnguloPID < ANG_MIN ) outPID = OCRNX_OFF;

          OCRnX = (uint8_t) outPID;

          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX - OCRNX_GIRO);
          setDutyB0(OCRnX - OCRNX_GIRO);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX + OCRNX_GIRO);
          setDutyB2(OCRnX + OCRNX_GIRO);

          flag_timer1 = 1; // Bandera de temporizacion
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
          if(AnguloPID > ANG_MAX || AnguloPID < ANG_MIN ) outPID = OCRNX_OFF;

          OCRnX = (uint8_t) outPID;

          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX + OCRNX_GIRO);
          setDutyB0(OCRnX + OCRNX_GIRO);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX - OCRNX_GIRO);
          setDutyB2(OCRnX - OCRNX_GIRO);

          flag_timer1 = 1; // Bandera de temporizacion
          break;
      case Retain_immobile:
          while (flag_timer1) { _delay_us(1);} //Bucle para temporizacion: Espera interrupcion del timer
          /*PID*/
          AnguloPID = getAngulo();
          error =  STATIC_SETPOINT - AnguloPID;
          outPID = pid(error);

          /*Saturacion del PWM*/
          if (outPID > OCRNX_MAX) {outPID = OCRNX_MAX;}
          if(outPID < OCRNX_MIN) {outPID = OCRNX_MIN;}
          if(AnguloPID > ANG_MAX || AnguloPID < ANG_MIN ) outPID = OCRNX_OFF;
          OCRnX = (uint8_t) outPID;
          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRnX);
          setDutyB0(OCRnX);
          /*MOTOR DERECHO */
          setDutyA2(OCRnX);
          setDutyB2(OCRnX);

          flag_timer1 = 1; // Bandera de temporizacion
          break;
      case Stop:
          /*MOTOR IZQUIERDO*/
          setDutyA0(OCRNX_OFF);
          setDutyB0(OCRNX_OFF);
          /*MOTOR DERECHO */
          setDutyA2(OCRNX_OFF);
          setDutyB2(OCRNX_OFF);
          return 0;
      default:
          break;
    }

  }
  return 0;
}

/*---------Interrupt routine service -------------*/

ISR(TIMER1_COMPA_vect){
    cli();
    flag_timer1 = 0;
    TIFR1 &=~ (1<<OCF1A); //apaga flag
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