#ifndef pid_h
  #define pid_h
  
  #include <stdlib.h>
  #include <stdio.h>

  volatile float e_sum = 0; // Suma de los errores
  volatile float e_ant = 0; // Error anterior
  volatile float Kp = 0; // Ganancia proporcional
  volatile float Ki = 0; // Ganancia integral
  volatile float Kd = 0; // Ganancia derivatiba
  volatile float Ts = 0;  // Tiempo de muestreo [s]

  float pid(float e){
    /**
    * Implementacion de un controlador P+I+D
    * @param e: error actual
    * @returns u: accion de control
    **/
    float u = Kp * e + Ki * Ts * e_sum + Kd * (e - e_ant) / Ts;
    e_sum += e;
    e_ant = e;
    return u;
  }

  void setControllerGains(float Kp_, float Ki_, float Kd_){
    /**
    * Funcion para setear las ganancias del controlador
    * @param Kp_: ganancia proporcional
    * @param Ki_: ganancia integral
    * @param Kd_: ganancia derivatiba
    **/
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
  }

  void setSamplingTime(float Ts_){
    /**
    * Funcion para setear el tiempo de muestreo
    * @param T_: tiempo de muestreo en [ms]
    **/
    Ts = Ts_ / 1000;
  }
#endif
