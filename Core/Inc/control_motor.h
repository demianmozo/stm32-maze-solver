#ifndef __CONTROL_MOTOR_H
#define __CONTROL_MOTOR_H

#include "main.h"

/* Definiciones para control de motores */
#define VELOCIDAD_AVANCE 700 // 70% de 1000 (período del timer) VELOCIDAD CUANDO AVANZA
#define VELOCIDAD_GIRO 1000  // 100% de 1000 (período del timer) VELOCIDAD CUANDO GIRA

/* Tiempos de giro en milisegundos (ajustar según calibración) */
#define TIEMPO_GIRO_90 500   // Tiempo para giro de 90 grados
#define TIEMPO_GIRO_180 1000 // Tiempo para giro de 180 grados

/* Estados de motor según tabla de control */
typedef enum
{
    MOTOR_AVANCE = 0, // M1=0, M0=1
    MOTOR_RETROCESO,  // M1=1, M0=0
    MOTOR_FRENADO     // M1=0, M0=0 o M1=1, M0=1
} motor_estado_t;

/* Prototipos de funciones */
void control_motor_init(void);
void avanza(void);
void gira90izq(void);
void gira90der(void);
void gira180(void);
void termino(void);

/* Funciones auxiliares */
void set_motor_izq(motor_estado_t estado, uint16_t pwm);
void set_motor_der(motor_estado_t estado, uint16_t pwm);

#endif /* __CONTROL_MOTOR_H */