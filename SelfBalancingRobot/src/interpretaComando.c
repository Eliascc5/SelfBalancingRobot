
#include"interpretaComando.h"


void InterpretaComando(void){
	volatile int aux_i;
  volatile double aux_f;
	switch(comando[0]){
    case 'S': //SETPOINT
          aux_i = atoi(&comando[1]); // lee desde el caracter 1 hasta el caracter nulo y lo convierte en int
          if (aux_i <= ANG_MAX && aux_i >= ANG_MIN){
              SETPOINT = aux_i;
          }else{
          printf("ST muy grande:%d\n",SETPOINT);
          }
          break;
		case 'P':
		   aux_f = atof(&comando[1]); // string to float
       setKP(aux_f);
		   break;
    case 'I':
		   aux_f = atof(&comando[1]); // string to float
       setKI(aux_f);
		   break;
    case 'D':
 			 aux_f = atof(&comando[1]); // string to float
       setKD(aux_f);
 			 break;
    case 'F':
      SETPOINT = 85.0; //Esto hay que sacarlo de aca y setearlo en el interpreta comando
      estado_s = Move_forward;
      break;
    case 'B':
      SETPOINT = 95.0; //Esto hay que sacarlo de aca y setearlo en el interpreta comando
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