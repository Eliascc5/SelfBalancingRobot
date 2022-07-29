
#include <avr/interrupt.h>

//Primer registro asociado PCICR, habilita los pines con iterrupciones por cambio
#define habilita_pines_intpc_0_7 (PCICR|=1<<PCIE0)    //PORTB
#define habilita_pines_intpc_8_14 (PCICR|=1<<PCIE1)   //PORTC
#define habilita_pines_intpc_15_23 (PCICR|=1<<PCIE2)   //PORTD
//Segundo registro asociado PCIFR, contiene a los flags de los grupos de pines con int por cambio

#define flag_PCINT0_apagado (PCIFR &=~(1<<PCIF0)) //Apaga el flag de cualquier interrupcion del grupo 1 (Pines 0-7)
#define flag_PCINT1_apagado (PCIFR &=~(1<<PCIF1)) //Apaga el flag de cualquier interrupcion del grupo 2 (Pines 8-14)
#define flag_PCINT2_apagado (PCIFR &=~(1<<PCIF2)) //Apaga el flag de cualquier interrupcion del grupo 3 (Pines 14-23)


//Por ultimo.. Habilita un pin especifico de alguno de los grupos de interrupcion por cambio.

//PCMSK0 corresponde al PORTB
#define habilita_intpc_B0 (PCMSK0=(1<<PCINT0))
#define habilita_intpc_B1 (PCMSK0=(1<<PCINT1))
#define habilita_intpc_B2 (PCMSK0=(1<<PCINT2))
#define habilita_intpc_B3 (PCMSK0=(1<<PCINT3))
#define habilita_intpc_B4 (PCMSK0=(1<<PCINT4))
#define habilita_intpc_B5 (PCMSK0=(1<<PCINT5))

//PCMSK1 corresponde al PORTC

#define habilita_intpc_C0 (PCMSK1=(1<<PCINT8))
#define habilita_intpc_C1 (PCMSK1=(1<<PCINT9))
#define habilita_intpc_C2 (PCMSK1=(1<<PCINT10))
#define habilita_intpc_C3 (PCMSK1=(1<<PCINT11))
#define habilita_intpc_C4 (PCMSK1=(1<<PCINT12))
#define habilita_intpc_C5 (PCMSK1=(1<<PCINT13))

//PCMSK2 corresponde al PORTD

#define habilita_intpc_D0 (PCMSK2=(1<<PCINT16))
#define habilita_intpc_D1 (PCMSK2=(1<<PCINT17))
#define habilita_intpc_D2 (PCMSK2=(1<<PCINT18))
#define habilita_intpc_D3 (PCMSK2=(1<<PCINT19))
#define habilita_intpc_D4 (PCMSK2=(1<<PCINT20))
#define habilita_intpc_D5 (PCMSK2=(1<<PCINT21))
#define habilita_intpc_D6 (PCMSK2=(1<<PCINT22))
#define habilita_intpc_D7 (PCMSK2=(1<<PCINT23))
