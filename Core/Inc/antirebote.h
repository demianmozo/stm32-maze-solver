/**
 * @file antirebote.h
 * @brief Módulo antirebote para botones y sensores.
 * @defgroup ANTIREBOTE Módulo Antirebote
 * @{
 * @details Este módulo implementa una función genérica de antirebote para cualquier pin GPIO.
 * @author demianmozo
 * @date 2025-06-08
 * @version 1.0
 */

#ifndef __ANTIREBOTE_H
#define __ANTIREBOTE_H

#include "main.h"
#include <stdbool.h>

#define TREBOTES 50 ///< Tiempo de filtrado antirebote en milisegundos

/**
 * @brief Función genérica de antirebote para cualquier pin GPIO
 * @param puerto Puntero al puerto GPIO (ej: GPIOA, GPIOB, etc.)
 * @param pin Máscara del pin GPIO (ej: GPIO_PIN_0, GPIO_PIN_1, etc.)
 * @return true si se detectó una pulsación válida (transición HIGH→LOW), false en caso contrario
 */
bool antirebote(GPIO_TypeDef *puerto, uint16_t pin);

#endif /* __ANTIREBOTE_H */