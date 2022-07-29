#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdio.h>
#include <math.h>
/*---------------------*/
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
/*---------------------*/
/*Drivers*/
#include "timeUno.h"
#include "i2c_master.h"
#include "mpu_6050.h"
#include "i2c_DEV.h"
#include "UART_ATmega0.h"
#include "pid.h"
#include "gpio_register_atmega328p.h"
#include "processing_data.h"
#include "interpretaComando.h"




#endif
