
#ifndef DEV_ADDR
	#define DEV_ADDR 0x68
#endif
#include <stdint.h>
#include <util/delay.h>
uint8_t DEV_rdy(uint8_t ad0);
uint8_t DEV_write(uint8_t ad0, uint8_t address, uint8_t dato);
//uint8_t DEV_write_N(uint8_t ad0, uint8_t address, uint8_t *data);
//uint8_t DEV_read(uint8_t ad0, uint8_t address);
uint16_t DEV_read16(uint8_t ad0, uint8_t address); //lee entero 16 bits;
//uint8_t DEV_read_fast(uint8_t ad0);
