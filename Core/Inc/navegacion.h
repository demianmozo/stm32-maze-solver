#ifndef __NAVEGACION_H
#define __NAVEGACION_H

#include <stdint.h>
#include "control_motor.h"
#include "laberinto.h"

/**
 * @brief Calcula la mejor dirección basándose en los pesos del laberinto
 */
brujula calcular_mejor_direccion(uint8_t fila_actual, uint8_t columna_actual);

/**
 * @brief Ejecuta el movimiento necesario para ir del sentido actual al deseado
 */
brujula ejecutar_movimiento(brujula sentido_actual, brujula sentido_deseado);

#endif /* __NAVEGACION_H */