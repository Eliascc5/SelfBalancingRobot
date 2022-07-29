#ifndef INTERPRETACOMANDO_H
#define INTERPRETACOMANDO_H


#define ANG_MAX 130
#define ANG_MIN 50

/*Estados posibles de la maquina de estados*/
enum tEstados_m{Start, Move_forward, Move_backward, Turn_right, Turn_left, Retain_immobile, Stop}estado_s;
extern char comando[30];
extern volatile float SETPOINT;
void InterpretaComando(void);

#endif