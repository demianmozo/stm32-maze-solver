/**
 * @file laberinto.c
 * @brief Implementación del módulo de control del laberinto
 * @author demianmozo
 */

#include "laberinto.h"

/** @defgroup Laberinto_Variables Variables del laberinto
 * @brief Variables estáticas para representación interna del laberinto
 * @{
 */

/** @brief Array bidimensional que representa el laberinto completo */
static casilla_t laberinto[TAMAÑO_LABERINTO][TAMAÑO_LABERINTO];

/**
 * @}
 */

/**
 * @brief Inicializa el laberinto con pesos por distancia Manhattan y sin muros
 * @details Configura cada casilla con:
 * - Identificación de posición (fila, columna)
 * - Peso inicial calculado como distancia Manhattan a la meta
 * - Sin muros inicialmente (laberinto abierto)
 * - Meta en (1,1) con peso 0
 */
void laberinto_init(void)
{
    // Inicializar cada casilla
    for (uint8_t fila = 1; fila <= TAMAÑO_LABERINTO; fila++)
    {
        for (uint8_t columna = 1; columna <= TAMAÑO_LABERINTO; columna++)
        {

            // Configurar identificación
            laberinto[fila - 1][columna - 1].posicion.fila = fila;
            laberinto[fila - 1][columna - 1].posicion.columna = columna;

            // Calcular peso inicial (distancia jaja sape a la meta)
            uint8_t distancia_fila = (fila > POSICION_META_FILA) ? (fila - POSICION_META_FILA) : (POSICION_META_FILA - fila);
            uint8_t distancia_columna = (columna > POSICION_META_COLUMNA) ? (columna - POSICION_META_COLUMNA) : (POSICION_META_COLUMNA - columna);

            laberinto[fila - 1][columna - 1].peso = distancia_fila + distancia_columna;

            // Inicializar sin muros
            for (uint8_t dir = 0; dir < 4; dir++)
            {
                laberinto[fila - 1][columna - 1].muros[dir] = false;
            }
        }
    }

    // La meta tiene peso 0
    laberinto[POSICION_META_FILA - 1][POSICION_META_COLUMNA - 1].peso = 0;
}

/**
 * @brief Obtiene el peso de una casilla específica
 * @param fila Fila de la casilla 
 * @param columna Columna de la casilla 
 * @return Peso de la casilla, PESO_MAXIMO si posición inválida
 */
uint8_t laberinto_get_peso(uint8_t fila, uint8_t columna)
{
    if (!laberinto_posicion_valida(fila, columna))
    {
        return PESO_MAXIMO;
    }

    return laberinto[fila - 1][columna - 1].peso;
}

/**
 * @brief Registra un muro detectado y actualiza el laberinto
 * @param fila Fila donde se detectó el muro
 * @param columna Columna donde se detectó el muro
 * @param direccion Dirección del muro detectado
 * @details Proceso completo:
 * 1. Marca el muro en la casilla actual
 * 2. Marca el muro opuesto en la casilla adyacente
 * 3. Recalcula todos los pesos con Flood Fill
 */
void laberinto_set_muro(uint8_t fila, uint8_t columna, brujula direccion)
{
    if (!laberinto_posicion_valida(fila, columna))
    {
        return;
    }

    // Marcar muro en casilla actual
    laberinto[fila - 1][columna - 1].muros[direccion] = true;

    // Marcar muro en casilla adyacente (si existe)
    posicion_t pos_adyacente = laberinto_get_posicion_adyacente(
        (posicion_t){fila, columna}, direccion);

    if (laberinto_posicion_valida(pos_adyacente.fila, pos_adyacente.columna))
    {
        // Dirección opuesta
        brujula direccion_opuesta = (direccion + 2) % 4;
        laberinto[pos_adyacente.fila - 1][pos_adyacente.columna - 1].muros[direccion_opuesta] = true;
    }

    // Recalcular pesos después de agregar muro
    laberinto_recalcular_pesos();
}

/**
 * @brief Implementa el algoritmo Flood Fill para recalcular pesos
 * @details Algoritmo iterativo que propaga pesos desde la meta:
 * 1. Mantiene la meta con peso 0
 * 2. Para cada casilla, calcula peso = menor_peso_adyacente + 1
 * 3. Respeta muros existentes (no considera casillas bloqueadas)
 * 4. Itera hasta convergencia o máximo de iteraciones
 *
 * @note Protección contra bucles infinitos con MAX_ITERACIONES = 20
 */
void laberinto_recalcular_pesos(void)
{
    bool cambio_detectado = true;
    uint8_t iteraciones = 0;
    const uint8_t MAX_ITERACIONES = 20; // Evitar bucles infinitos Protección contra boludos

    // Algoritmo Flood Fill iterativo
    while (cambio_detectado && iteraciones < MAX_ITERACIONES)
    {
        cambio_detectado = false;
        iteraciones++;

        for (uint8_t fila = 1; fila <= TAMAÑO_LABERINTO; fila++) // se propaga desde (1,1) hacia afuera
        {
            for (uint8_t columna = 1; columna <= TAMAÑO_LABERINTO; columna++)
            {

                // No recalcular la meta
                if (fila == POSICION_META_FILA && columna == POSICION_META_COLUMNA)
                {
                    continue;
                }

                uint8_t peso_minimo = PESO_MAXIMO;

                // Verificar todas las direcciones adyacentes
                for (brujula dir = norte; dir <= oeste; dir++)
                {

                    // Saltar si hay muro en esta dirección
                    if (laberinto[fila - 1][columna - 1].muros[dir])
                    {
                        continue;
                    }

                    posicion_t pos_adyacente = laberinto_get_posicion_adyacente(
                        (posicion_t){fila, columna}, dir);

                    if (laberinto_posicion_valida(pos_adyacente.fila, pos_adyacente.columna))
                    {
                        uint8_t peso_adyacente = laberinto_get_peso(
                            pos_adyacente.fila, pos_adyacente.columna);

                        if (peso_adyacente < peso_minimo)
                        {
                            peso_minimo = peso_adyacente;
                        }
                    }
                }

                // Nuevo peso = menor peso adyacente + 1
                uint8_t nuevo_peso = (peso_minimo == PESO_MAXIMO) ? PESO_MAXIMO : peso_minimo + 1;

                // Actualizar si hay cambio
                if (nuevo_peso != laberinto[fila - 1][columna - 1].peso)
                {
                    laberinto[fila - 1][columna - 1].peso = nuevo_peso;
                    cambio_detectado = true;
                }
            }
        }
    }
}

/**
 * @brief Verifica si existe un muro en una dirección específica
 * @param fila Fila de la casilla a verificar
 * @param columna Columna de la casilla a verificar
 * @param direccion Dirección a verificar (norte, este, sur, oeste)
 * @return true si hay muro o posición inválida, false si está libre
 */
bool laberinto_hay_muro(uint8_t fila, uint8_t columna, brujula direccion)
{
    if (!laberinto_posicion_valida(fila, columna))
    {
        return true; // Considerar bordes como muros
    }

    return laberinto[fila - 1][columna - 1].muros[direccion];
}

/**
 * @brief Calcula la posición adyacente en una dirección dada
 * @param pos_actual Posición de referencia
 * @param direccion Dirección de movimiento (norte, este, sur, oeste)
 * @return Nueva posición calculada (puede ser inválida si está fuera del laberinto)
 */
posicion_t laberinto_get_posicion_adyacente(posicion_t pos_actual, brujula direccion)
{
    posicion_t nueva_pos = pos_actual;

    switch (direccion)
    {
    case norte:
        nueva_pos.fila = pos_actual.fila - 1;
        break;
    case este:
        nueva_pos.columna = pos_actual.columna + 1;
        break;
    case sur:
        nueva_pos.fila = pos_actual.fila + 1;
        break;
    case oeste:
        nueva_pos.columna = pos_actual.columna - 1;
        break;
    }

    return nueva_pos;
}

/**
 * @brief Valida si una posición está dentro de los límites del laberinto
 * @param fila Fila a validar
 * @param columna Columna a validar
 * @return true si la posición es válida (1 ≤ fila,columna ≤ 4), false en caso contrario
 */
bool laberinto_posicion_valida(uint8_t fila, uint8_t columna)
{
    return (fila >= 1 && fila <= TAMAÑO_LABERINTO &&
            columna >= 1 && columna <= TAMAÑO_LABERINTO);
}