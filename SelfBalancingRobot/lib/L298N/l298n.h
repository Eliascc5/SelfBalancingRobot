
#include"gpio_register_atmega328p.h"
//PINES ASOCIADOS A LOS DOS TIMMER DE 8 BITS

#define MOTOR_DIRECTION_FORWARD 1
#define MOTOR_DIRECTION_BACKWARD 0

void motor_right(uint8_t direction) {
	if (direction == MOTOR_DIRECTION_FORWARD) {
		D6_apagado;
		D5_encendido;
	} else if (direction == MOTOR_DIRECTION_BACKWARD) {
		D6_encendido;
		D5_apagado;

	}
}

void motor_left(uint8_t direction) {
	if (direction == MOTOR_DIRECTION_FORWARD) {
		D3_apagado;
		B3_encendido;

	} else if (direction == MOTOR_DIRECTION_BACKWARD) {
		D3_encendido;
		B3_apagado;

	}


}
