#ifndef CONTROL_LINEARECTA_H
#define CONTROL_LINEARECTA_H

#include "main.h"
#include <stdint.h>

// Definiciones
#define BUFFER_TOTAL 200
#define BUFFER_MINIMO 100
#define MUESTRAS 10

// Variables externas
extern uint16_t dma_buffer[BUFFER_TOTAL];
extern uint16_t sensor_izq_avg;
extern uint16_t sensor_der_avg;

// Declaraciones de funciones
void auto_calibracion(void);
void promediar_sensores(uint16_t *buffer);
void controlar_linea_recta(void);
void correccion_izquierda(void);
void correccion_derecha(void);

#endif