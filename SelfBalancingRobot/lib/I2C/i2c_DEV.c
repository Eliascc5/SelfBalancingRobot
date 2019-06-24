  #include <stdint.h>
  #include "i2c_master.h"
  #include "i2c_DEV.h"

  uint8_t DEV_rdy(uint8_t ad0){
    uint8_t ack;
    ack = i2c_start((DEV_ADDR|(ad0&7))<<1);  // Condición de START
    return !ack;  // si ack=0 exito, vuelve 1
  }

  uint8_t DEV_write(uint8_t ad0, uint8_t address, uint8_t dato){

        uint8_t ack;
        while(!DEV_rdy(ad0)){
          _delay_us(1);// Espera hasta que el sensor 0 esté listo
        }
        ack = i2c_start((DEV_ADDR|(ad0&7))<<1);      // Condición de START
        i2c_write(address);           // Escribe byte de dirección de registro a acceder dentro del sensor
        i2c_write(dato);              // Escribe el dato en la dirección escrita
        i2c_stop();                   // Condición de STOP
  	  return ack;
  }

  /* Escribe N BYTES *data en DIRECCIONES CONSECUTIVAS a partir de address */
  uint8_t DEV_write_N(uint8_t ad0, uint8_t address, uint8_t *data){
  	  uint8_t ack;
        while(!DEV_rdy(ad0)){
           _delay_us(1);// espera hasta que el sensor m esté listo
        }
        ack = i2c_start((DEV_ADDR|(ad0&7))<<1); //dir es 0 o 1 en MPU6050, pero reservamos 3 bits
        i2c_write(address);                     // escribe byte de dirección a acceder dentro del sensor
        while(*data!=0)                         // mientras dato sea distinto de NULL (fin de cadena)
        {
              i2c_write(*data++);               // escribe dato e incrementa puntero en uno
        }
        i2c_stop();                             // condición de STOP
  	  return ack;
  }

  uint8_t DEV_read(uint8_t ad0, uint8_t address)
  {
  	  uint8_t dato;
        while(!DEV_rdy(ad0))
        {
               _delay_us(1);// espera hasta que el sensor 0 esté listo
        }
        i2c_start((DEV_ADDR|(ad0&7))<<1); //dir es 0 o 1 en MPU6050, pero reservamos 3 bits
        i2c_write(address);                     // escribe byte de dirección a acceder dentro del sensor
        i2c_start((DEV_ADDR|(ad0&7))<<1|1); //lectura
        dato  = i2c_read(0);           // lee dato de i2c
        i2c_stop();                   // condición de STOP
        return(dato);                 // devuelve dato leído
  }

  uint16_t DEV_read16(uint8_t ad0, uint8_t address){ //lee entero 16 bits

  	uint16_t dato;
  	while(!DEV_rdy(ad0))
  	{
  		 _delay_us(1);// espera hasta que el sensor 0 esté listo
  	}
  	i2c_start((DEV_ADDR|(ad0&7))<<1);    //dir es 0 o 1 en MPU6050, pero reservamos 3 bits
  	i2c_write(address);                 // escribe byte de dirección a acceder dentro del sensor
  	i2c_start((DEV_ADDR|(ad0&7))<<1|1); //lectura
  	dato = i2c_read(1);           // lee dato de i2c
  	dato<<=8;
  	dato|= i2c_read(0);
  	i2c_stop();                   // condición de STOP
  	return(dato);                 // devuelve dato leído
  }

  /*            Devuelve el byte leído de dirección ACTUAL.
                Direcciona sensor m según pin AD0.
                No usada en este programa.                                     */

  uint8_t DEV_read_fast(uint8_t ad0) // Lee desde la posición actual.
  {
  	  uint8_t dato;
        while(!DEV_rdy(ad0))
        {
             _delay_us(1);  // espera hasta que el sensor m esté listo
        }
        dato=i2c_read(0);             // lee dato de i2c
        i2c_stop();                   // condición de STOP
        return(dato);
  }
