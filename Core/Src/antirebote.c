/**
 * @file antirebote.c
 * @brief Implementación del módulo antirebote.
 */

#include "antirebote.h"
/**
 * @brief Función genérica de antirebote para cualquier pin GPIO
 * @ingroup ANTIREBOTE
 */
bool antirebote(GPIO_TypeDef *puerto, uint16_t pin)
{
    // Variables estáticas para guardar el último estado y si fue inicializado
    static GPIO_PinState
        ultimas_lecturas[16]; // Para hasta 16 pines distintos uso un array, god
    static uint8_t inicializado[16] = {
        0}; // se inicializan en cero solo la 1era vez que llamas la funcion

    // Calcular el índice del pin a partir de la máscara (posición del bit)
    uint8_t index = 0;
    uint16_t mask = pin;
    while (mask > 1)
    {
        mask >>= 1; // Desplazar a la derecha el bit
        index++;    // Contar cuántos desplazamientos hicimos
    }

    GPIO_PinState lectura1, lectura2;
    bool pulsado = false;

    // Leer el estado actual del pin
    lectura1 =
        HAL_GPIO_ReadPin(puerto, pin); //  xq puerto es el puntero al pueertooo

    // Si no se había inicializado antes, guardar lectura inicial y salir
    if (!inicializado[index])
    { // se ejecuta solo la primera vez que se llama a
      // la función para ese pin.
        ultimas_lecturas[index] = lectura1;
        inicializado[index] = 1;
        return false;
    }

    // Si detectamos un cambio respecto a la última lectura válida
    if (lectura1 != ultimas_lecturas[index])
    {
        HAL_Delay(TREBOTES); // Esperar para filtrar rebotes

        // Leer el pin otra vez para confirmar que la lectura es estable
        lectura2 = HAL_GPIO_ReadPin(puerto, pin);

        // Si la segunda lectura coincide con la primera, es válida
        if (lectura1 == lectura2)
        {
            ultimas_lecturas[index] = lectura2; // Guardar lectura válida

            // Detectar si el botón fue presionado (activo en bajo)
            if (lectura2 == GPIO_PIN_RESET)
            {
                pulsado = true;
            }
        }
    }

    return pulsado;
}