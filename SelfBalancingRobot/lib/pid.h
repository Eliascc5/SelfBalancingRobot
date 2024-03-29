#ifndef pid_h
  #define pid_h

  #include <stdlib.h>
  #include <stdio.h>

  volatile double e_sum = 0; // Suma de los errores
  volatile double e_ant = 0; // Error anterior
  volatile double u;
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
    u = Kp * e + Ki * Ts * e_sum + Kd * (e - e_ant) / Ts;
    e_sum = e_sum + (int)e;
    // printf("PID %d, %d, %d, %d\n",(int) e_sum,  (int)e, (int)(e - e_ant), (int)u+127);
    e_ant = e;
    /*Saturacion de e_sum*/
    if (e_sum > 500) e_sum = 500;
    if (e_sum < -500) e_sum = -500;
    return (u+127);
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

  void setKP(double Kp_){
    /**
    * Funcion para setear las ganancias del controlador
    * @param Kp_: ganancia proporcional
    **/
    Kp = Kp_;
  }

  void setKI(double Ki_){
    /**
    * Funcion para setear las ganancias del controlador
    * @param Ki_: ganancia integral
    **/
    Ki = Ki_;
  }

  void setKD(double Kd_){
    /**
    * Funcion para setear las ganancias del controlador
    * @param Kd_: ganancia derivatiba
    **/
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
