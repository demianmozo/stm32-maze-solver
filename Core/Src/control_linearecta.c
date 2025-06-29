#include "control_linearecta.h"
#include "control_motor.h"
#include <stdbool.h>

// Variables globales
uint16_t sensor_izq_avg = 0;
uint16_t sensor_der_avg = 0;

extern volatile bool flag_linea_detectada;
extern volatile bool flag_muro_detectado;

// Umbrales dinámicos (se escriben nuevamente en calibración)
uint16_t izq_cerca = 400, izq_lejos = 4000, izq_centrado = 2200;
uint16_t der_cerca = 400, der_lejos = 4000, der_centrado = 2200;

bool calibrado = false;

// Interrupciones DMA
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    promediar_sensores(&dma_buffer[0]);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    promediar_sensores(&dma_buffer[BUFFER_MINIMO]);
}

// Función promediar se ejecuta constantemente en DMA
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

// Auto-calibración
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

// Control de línea recta
void controlar_linea_recta(void)
{
    if (!calibrado)
        return;

    // Determinar posición relativa
    bool muy_cerca_izq = (sensor_izq_avg < izq_cerca + 200);
    bool muy_cerca_der = (sensor_der_avg < der_cerca + 200);

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
