/* Este micro posee dos pines asociados a interrupciones externas (PD2/INTO y PD3/INT1)*/

#define INT0_test (PIND & (1<<PIND2))
#define INT1_test (PIND & (1<<PIND3))

//El registro EIMSK(External interrupt mask register) permite habilitar/deshabilitar las interrupciones externas INT0 e INT1

#define INT0_habilitada (EIMSK |= (1<<INT0))
#define INT0_deshabilitada (EIMSK &=~ (1<<INT0))

#define INT1_habilitada (EIMSK |= (1<<INT1))
#define INT1_deshabilitada (EIMSK &=~ (1<<INT1))

//El registro EICRA permite elegir el tipo de evento que levanta el "flag" de la interrupciones externas INT0 e INT1

#define INT0_evento_nivelBajo (EICRA &=~ (3<<ISC00))
#define INT0_evento_cualquierFlanco (EICRA |= (1<<ISC00))
#define INT0_evento_flancoBajada (EICRA |= (2<<ISC00))
#define INT0_evento_flancoSubida (EICRA |= (3<<ISC00))




//PARA INT 1
#define INT1_evento_nivelBajo (EICRA &=~ (3<<ISC10))
#define INT1_evento_cualquierFlanco (EICRA |= (1<<ISC10))
#define INT1_evento_flancoBajada (EICRA |= (2<<ISC10))
#define INT1_evento_flancoSubida (EICRA |= (3<<ISC10))





//El registro EIFR (External interurpt flag register) contiene a los flags de las interrupciones externas.

 #define flagINT0_apagado (EIFR &=~ (1<<INTF0))
 #define flagINT1_apagado (EIFR &=~ (1<<INTF1))
