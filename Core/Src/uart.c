/**
 * @file uart.c
 * @brief Implementación del módulo de comunicación UART para debugging
 * @author tamis
 * @date 2025-06-09
 *
 * @note Configuración: Baud rate 115200, 8 bits, sin paridad, 1 stop bit
 */

// nota: Baud rate: 115200

#include "main.h"
#include "uart.h"
#include <stdio.h>  //Nos permite usar la función sprintf
#include <string.h> //Nos permite usar la función strcat
#include <stdint.h>

/** @defgroup UART_Variables Variables de comunicación UART
 * @brief Variables globales para manejo de comunicación serie
 * @{
 */

/** @brief Buffer de mensaje para transmisión (máximo 16 caracteres) */
char mensaje[16];

/** @brief Timeout para transmisión UART en milisegundos */
const uint8_t delay = 50;

/** @brief Buffer para recepción de datos por interrupción */
uint8_t buffer[16];

/**
 * @}
 */

/**
 * @brief Transmite el mensaje actual por UART
 * @details Funcionamiento:
 * 1. Agrega terminador CRLF ("\r\n") al mensaje
 * 2. Transmite usando HAL_UART_Transmit con timeout
 * 3. Utiliza transmisión bloqueante para garantizar envío
 *
 * @note Requiere que la variable global 'mensaje' esté configurada previamente
 * @warning Transmisión bloqueante - puede causar delay de hasta 50ms
 */
void Transmision(void)
{
    strcat(mensaje, "\r\n");
    HAL_UART_Transmit(&huart5, (uint8_t *)mensaje, strlen(mensaje), delay);
}

/**
 * @brief Inicializa la comunicación UART y envía mensaje de conexión
 * @details Secuencia de inicialización:
 * 1. Configura recepción por interrupciones
 * 2. Limpia buffer de mensaje
 * 3. Envía línea en blanco para sincronización
 * 4. Transmite mensaje de confirmación "UART conectada"
 * 5. Limpia buffer para uso posterior
 *
 * @note Se ejecuta una sola vez al inicio del programa
 * @note Habilita recepción continua por interrupciones
 */
void Inicializar_UART(void)
{
    HAL_UART_Receive_IT(&huart5, buffer, sizeof(buffer));
    mensaje[0] = '\r';
    mensaje[1] = '\n';
    mensaje[2] = '\0';
    Transmision();
    strcpy(mensaje, "UART conectada");
    Transmision();
    mensaje[0] = '\0';
}
/*-----------------------------------------------------------------------------------
en main.c

#include "uart.h"

#include <string.h> //para mandar mensajes por la UART
 Private variables
inicializar la uart
      Inicializar_UART();
para transmitir con UART
    strcpy(mensaje, "M");
    Transmision(); */
