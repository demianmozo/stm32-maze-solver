/**
 * @file laberinto.h
 * @brief Módulo de control del laberinto
 * @author demianmozo
 * @date 2025-06-10
 * @version 1.0
 *
 * Implementa un laberinto de 4x4 casillas con algoritmo de llenado para
 * encontrar el camino óptimo desde la posición inicial (4,4) hasta
 * la meta (1,1).
 */

#ifndef __LABERINTO_H
#define __LABERINTO_H

#include <stdint.h>
#include <stdbool.h>
#include "control_motor.h" // Para usar el tipo brujula del colo

/* Configuración del laberinto */
#define TAMAÑO_LABERINTO 4        ///< Dimensión del laberinto (4x4)
#define PESO_MAXIMO 255           ///< Peso máximo para muros
#define POSICION_INICIO_FILA 4    ///< Fila de inicio (4,4) LO DEJAMOS GENERICO PARA EL SASE
#define POSICION_INICIO_COLUMNA 4 ///< Columna de inicio (4,4)
#define POSICION_META_FILA 1      ///< Fila de meta (1,1)
#define POSICION_META_COLUMNA 1   ///< Columna de meta (1,1)

/**
 * @brief Estructura que representa una posición en el laberinto
 */
typedef struct
{
    uint8_t fila;    ///< Fila (1-4)
    uint8_t columna; ///< Columna (1-4)
} posicion_t;

/**
 * @brief Estructura que representa una casilla con todas sus características
 */
typedef struct
{
    posicion_t posicion; ///< Identificación de la casilla (fila, columna)
    uint8_t peso;        ///< Peso actual de la casilla (distancia a meta)
    bool muros[4];       ///< Muros en cada dirección [norte, este, sur, oeste]
} casilla_t;

/**
 * @defgroup Laberinto Control del Laberinto
 * @brief Funciones para manejo del laberinto y algoritmo Flood Fill
 * @{
 */

/**
 * @brief Inicializa el laberinto con pesos ideales y sin muros
 * @details Configura el laberinto con pesos calculados como distancia Manhattan
 *          desde cada casilla hasta la meta (1,1). Inicialmente no hay muros.
 */
void laberinto_init(void);

/**
 * @brief Obtiene el peso de una casilla específica
 * @param fila Fila de la casilla (1-4)
 * @param columna Columna de la casilla (1-4)
 * @return Peso de la casilla, PESO_MAXIMO si hay muro o posición inválida
 */
uint8_t laberinto_get_peso(uint8_t fila, uint8_t columna);

/**
 * @brief Registra un muro entre dos casillas adyacentes
 * @param fila Fila de la casilla actual
 * @param columna Columna de la casilla actual
 * @param direccion Dirección donde se detectó el muro (usando tipo brujula)
 * @details Actualiza los muros en ambas casillas afectadas y recalcula pesos
 */
void laberinto_set_muro(uint8_t fila, uint8_t columna, brujula direccion);

/**
 * @brief Recalcula todos los pesos del laberinto usando Flood Fill
 * @details Propaga los pesos desde la meta hacia todas las casillas,
 *          respetando los muros conocidos
 */
void laberinto_recalcular_pesos(void);


/**
 * @brief Verifica si hay un muro en una dirección específica
 * @param fila Fila de la casilla
 * @param columna Columna de la casilla
 * @param direccion Dirección a verificar (usando tipo brujula)
 * @return true si hay muro, false si no hay muro o posición inválida
 */
bool laberinto_hay_muro(uint8_t fila, uint8_t columna, brujula direccion);

/**
 * @brief Obtiene la posición adyacente en una dirección
 * @param pos_actual Posición actual
 * @param direccion Dirección de movimiento (usando tipo brujula)
 * @return Nueva posición, o posición inválida si está fuera del laberinto
 */
posicion_t laberinto_get_posicion_adyacente(posicion_t pos_actual, brujula direccion);

/**
 * @brief Verifica si una posición es válida dentro del laberinto PARA HACERLO SIN MUROS EN LOS BORDES
 * @param fila Fila a verificar
 * @param columna Columna a verificar
 * @return true si la posición es válida (1-4, 1-4)
 */
bool laberinto_posicion_valida(uint8_t fila, uint8_t columna);

/** @} */ // fin grupo Laberinto

#endif /* __LABERINTO_H */