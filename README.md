# 🚀 Proyecto Final Microcontroladores y Electronica de Potencia 🚀
 Se plantea la fabricacion y control de un robot auto balanceado. Aqui, se encuentran los drivers del robot 
### Notas 📋 
* Se planteo un control PID para estabilizar el robot.
* Para hacer girar al robot se dan consignas complementarias a los motores (Ej: una salida del PID igual a 100 se convierte en Motor_I = 80 y Motor_D = 120) 
* Para hacer avanzar/retroceder al robot se le da una consigna distinta de la de equilibrio (90°) y se espera que el robot semueva para mantener el nuevo setPoint.
* Cabe destacar que aun no se ha logrado sintonizar el PID por lo tanto las consideraciones de movimiento antes mencionadas sonhipoteticas
### Lenguaje 📦
 C
### Materiales 🛠️
* ATMEGA328p
* MPU6050
* 2 DC Motors
* L298N 
### Autores ✒️
* **Correa Elias** 
* **Pino Demichelis Jeremias**


