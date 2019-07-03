
// Originales en https://github.com/g4lvanix/I2C-master-lib

/*
TWI-> "two wire interface":  SDA(datos),SCL(señal de clock) ( La comunicacion bi-direccional)
pag:206- Datasheet Atmega

TWDR-> Contiene la direccion o el byte de datos a ser transmitidos(registro de desplazamiento)
      (tambien podria ser la direccion o el byte de datos a ser recibidos)

Bit rate generator unit ---
Esta unidad controla el periodo de SCL cuando opera en un modo MAESTRO..El mismo es controlado
seteando TWBR (TWI bit rate register) y el prescaler.

TWSR-> (Status register)
TWCR-> Registro de control de TWI
TWEN-> Bit de enable de TWI

*/
  #ifndef F_CPU
  #define F_CPU 16000000UL
  #endif

  #include <avr/io.h>
  #include <util/twi.h>

  #include "i2c_master.h"

  #define Prescaler 1   //1,4,16,64 (Tabla pag 232)

  void i2c_wait(){
	   while ((TWCR & (1<<TWINT)) == 0);   //espera mientras el  bit de interrupcion sea 0
  }



  void i2c_init(uint32_t F_SCL){

    uint32_t TWBR_val;
    PORTC|=((1<<4)|(1<<5));                                   //Activa resistencias de pull-up para los pines SDA Y SCL
  	TWBR_val = (((F_CPU / F_SCL) - 16 ) / (Prescaler * 2));  //FRECUENCIA DE SCL F_SCL (Pag 213)
  	TWBR = (uint8_t)TWBR_val;  //Casting
  }


  //Incluye el bit de START y la escritura de address

  uint8_t i2c_start(uint8_t address){
    //TWCR (Registro de control)

  	TWCR = 0;             //Reset al registro de control de TWI (habilita la interfaz TWI)
  	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);  	//Transmite la condicion de START (Pag 217- datasheet-Atmega)

    //while(!(TWCR & (1<<TWINT)));
    i2c_wait();	             //Espera un flag-set en TWINT; Esto indica que la condicion de STAR ha sido transimitida
  	if((TWSR & 0xF8) != TW_START){  // Chequea el valor de TWSR.. Si ambos valores son iguales devuelve ERROR
      return 1;
    }

  	TWDR = address;   //Carga la direccion del esclavo en el registro de datos TWDR

  	TWCR = (1<<TWINT) | (1<<TWEN); //Comienza la transmision de la direccion

    i2c_wait();              //Espera un flag-set en TWINT; Esto indica que la direccion del esclavo ha sido transimitida
  	//Chequea el valor de TWI status register
  	uint8_t twst = TW_STATUS & 0xF8;     //Pag 216 datasheet
  	if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK))   //MT (Master transmitter) MR (master receiver)
    return 1;	  //Error

    return 0;   // exito
  }

    //Escritura de 1 byte de datos

    uint8_t i2c_write(uint8_t data){

  	TWDR = data;   //Carga el dato en el registro de datos TWDR

  	TWCR = (1<<TWINT) | (1<<TWEN);  //Set en TWINT y TWEN (comienza la transmision del dato)

  	//while( !(TWCR & (1<<TWINT)) );
    i2c_wait();           //Espera un flag-set en TWINT, esto indica que el dato ha sido transmitido
  	if((TWSR & 0xF8) != TW_MT_DATA_ACK){  //Verifica el TWSR a  TW_MT_DATA_ACK devuelve NACK
    return 1;  //NACK
    }
  	return 0;
  }

  //Lectura de una byte del bus

  uint8_t i2c_read(uint8_t ack){

  	TWCR = (1<<TWINT)|(1<<TWEN)|(ack<<TWEA); //Comienza el modelo ACK de TWI despues de la recepcion del dato

  	//while( !(TWCR & (1<<TWINT)) );
    i2c_wait();             //Espera un flag-set en TWINT, esto indica que el dato ha sido transmitido
  	return TWDR;  //Devuelve el dato recibido en TWDR
  }

  //Tranmite la condicion de STOP
  void i2c_stop(void){
  	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
  }
