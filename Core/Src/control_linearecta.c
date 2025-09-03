/**
 * @file control_linearecta.c
 * @brief Implementación del control PID para seguimiento de línea recta
 * @author demianmozo
 */

#include "control_linearecta.h"
#include "control_motor.h"
#include <stdbool.h>

/** @defgroup ControlLinea_Variables Variables de control de línea
 * @brief Variables globales para el control de seguimiento de línea recta
 * @{
 */

/** @brief Promedio del sensor izquierdo (filtrado) */
uint16_t sensor_izq_avg = 0;
/** @brief Promedio del sensor derecho (filtrado) */
uint16_t sensor_der_avg = 0;

extern volatile bool flag_linea_detectada; ///< Flag de interrupción de línea
extern volatile bool flag_muro_detectado;  ///< Flag de interrupción de muro

/** @brief Umbrales dinámicos para sensor izquierdo */
uint16_t izq_cerca = 400, izq_lejos = 4000, izq_centrado = 2200;
/** @brief Umbrales dinámicos para sensor derecho */
uint16_t der_cerca = 400, der_lejos = 4000, der_centrado = 2200;

/** @brief Flag que indica si la calibración fue completada */
bool calibrado = false;

/**
 * @}
 */

/** @defgroup ControlLinea_DMA_Callbacks Callbacks DMA del ADC
 * @brief Funciones callback para procesamiento en tiempo real de sensores
 * @{
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    promediar_sensores(&dma_buffer[0]);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    promediar_sensores(&dma_buffer[BUFFER_MINIMO]);
}
/**
 * @}
 */

/**
 * @brief Calcula el promedio filtrado de los sensores IR
 * @param buffer Puntero al segmento del buffer DMA a procesar
 * @details Proceso de filtrado:
 * - Procesa MUESTRAS lecturas intercaladas (canal 8 y 9)
 * - Canal 8 (PB0): Sensor derecho
 * - Canal 9 (PB1): Sensor izquierdo
 * - Calcula promedio aritmético para reducir ruido
 * - Actualiza variables globales sensor_der_avg y sensor_izq_avg
 *
 * @note Se ejecuta constantemente en DMA para actualización en tiempo real
 */
void promediar_sensores(uint16_t *buffer)
{
    uint32_t izq_sum = 0, der_sum = 0;

    // Canal 8 (derecho), Canal 9 (izquierdo)
    for (int i = 0; i < MUESTRAS; ++i)
    {
        der_sum += buffer[0]; // Canal 8 (PB0)
        izq_sum += buffer[1]; // Canal 9 (PB1)
        buffer += 2;          // Avanza 2 posiciones
    }

    sensor_der_avg = der_sum / MUESTRAS;
    sensor_izq_avg = izq_sum / MUESTRAS;
}

/**
 * @brief Ejecuta la secuencia de auto-calibración de sensores IR
 * @details Proceso de calibración en 3 etapas con indicación led:
 *
 * **Etapa 1 (LED Naranja):** Cerca de pared derecha (3s)
 * - Captura valor mínimo del sensor derecho (der_cerca)
 *
 * **Etapa 2 (LED Rojo):** Cerca de pared izquierda (3s)
 * - Captura valor mínimo del sensor izquierdo (izq_cerca)
 *
 * **Etapa 3 (LED Azul):** Centrado en pasillo (3s)
 * - Captura valores máximos de ambos sensores (izq_lejos, der_lejos)
 * - Calcula puntos medios para detección de centrado
 *
 * **Finalización (LED Verde):** Calibración completa
 * - Activa flag calibrado = true
 *
 * @warning Requiere intervención manual para posicionar el robot
 * @note Los umbrales se calculan dinámicamente según las condiciones del entorno
 */
void auto_calibracion(void)
{
    HAL_Delay(1000);

    // Medición 1: CERCA DE PARED DERECHA
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET); // Naranja
    HAL_Delay(3000);                                         // Tiempo para posicionar cerca de pared derecha

    der_cerca = sensor_der_avg;

    // Medición 2: CERCA DE PARED IZQUIERDA
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET); // Rojo
    HAL_Delay(3000);                                         // Tiempo para posicionar cerca de pared izquierda

    izq_cerca = sensor_izq_avg;

    // Medición 3: CENTRADO EN PASILLO
    HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_SET); // Azul
    HAL_Delay(3000);                                         // Tiempo para centrar

    izq_lejos = sensor_izq_avg;
    der_lejos = sensor_der_avg;

    // Calcular valores medios
    izq_centrado = (izq_cerca + izq_lejos) / 2;
    der_centrado = (der_cerca + der_lejos) / 2;

    // Calibración completa
    HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET); // Verde
    HAL_Delay(1000);
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

    calibrado = true;
}

/**
 * @brief Controla el seguimiento de línea recta usando sensores IR
 * @details Algoritmo de control reactivo:
 * 1. Verifica que la calibración esté completa
 * 2. Evalúa la proximidad a las paredes laterales
 * 3. Aplica correcciones direccionales según la situación:
 *    - Muy cerca de pared izquierda → Corrección derecha
 *    - Muy cerca de pared derecha → Corrección izquierda
 *    - Centrado → Avance recto
 *
 * @note Utiliza umbrales dinámicos calculados en calibración
 * @note Margen de seguridad de 200 unidades sobre valores de calibración
 * @warning No opera sin calibración previa (calibrado = false)
 */
void controlar_linea_recta(void)
{
    if (!calibrado)
        return;

    // Determinar posición relativa
    bool muy_cerca_izq = (sensor_izq_avg < izq_cerca + 100);
    bool muy_cerca_der = (sensor_der_avg < der_cerca + 100);

    if (muy_cerca_izq)
    {
        correccion_derecha(); // Alejarse de pared izquierda
    }
    else if (muy_cerca_der)
    {
        correccion_izquierda(); // Alejarse de pared derecha
    }
    else if (muy_cerca_izq)
    {
        correccion_derecha(); // Alejarse de pared izquierda
    }
    else
    {
        avanza(); // Ir recto si está centrado
    }
}
