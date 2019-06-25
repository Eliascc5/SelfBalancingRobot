#ifndef pid_h
  #define pid_h

  #include <stdlib.h>
  #include <stdio.h>

  volatile double e_sum = 0; // Suma de los errores
  volatile double e_ant = 0; // Error anterior
  volatile double Kp = 0; // Ganancia proporcional
  volatile double Ki = 0; // Ganancia integral
  volatile double Kd = 0; // Ganancia derivatiba
  volatile double Ts = 0;  // Tiempo de muestreo [s]

  double pid(double e){
    /**
    * Implementacion de un controlador P+I+D
    * @param e: error actual
    * @returns u: accion de control
    **/
    double u = Kp * e + Ki * Ts * e_sum + Kd * (e - e_ant) / Ts;
    e_sum += e;
    e_ant = e;
    return u;
  }

  void setControllerGains(double Kp_, double Ki_, double Kd_){
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

  void setSamplingTime(double Ts_){
    /**
    * Funcion para setear el tiempo de muestreo
    * @param T_: tiempo de muestreo en [ms]
    **/
    Ts = Ts_ / 1000.0;
  }
#endif
