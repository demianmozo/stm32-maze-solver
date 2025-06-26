/**
 * @file control_motor.c
 * @brief Implementación del módulo de control de motores.
 */
#include "control_motor.h"
#include <stdbool.h>

extern TIM_HandleTypeDef htim3;            // usa el timer 3 para PWM
extern volatile bool flag_linea_detectada; // flag linea
extern volatile bool flag_muro_detectado;  // flag muro


uint16_t velocidad_actual_izq = VELOCIDAD_AVANCE_IZQ;
uint16_t velocidad_actual_der = VELOCIDAD_AVANCE_DER;
uint16_t velocidad_giro_actual_izq = VELOCIDAD_GIRO_IZQ;
uint16_t velocidad_giro_actual_der = VELOCIDAD_GIRO_DER;

void activar_modo_sprint(void)
{
    velocidad_actual_izq = VELOCIDAD_SPRINT_IZQ;
    velocidad_actual_der = VELOCIDAD_SPRINT_DER;
}

/**
 * @brief Inicializa el control de motores
 */
void control_motor_init(void)
{
    // Iniciar PWM en ambos canales
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // Motor izquierdo (PC8)
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); // Motor derecho (PC9)

    // comienza yendo para adelante
    avanza();
}

/**
 * @brief Configura motor izquierdo
 * @param estado: MOTOR_AVANCE, MOTOR_RETROCESO o MOTOR_FRENADO
 * @param pwm: Valor PWM (0-1000)
 */
void set_motor_izq(motor_estado_t estado, uint16_t pwm)
{
    switch (estado)
    {
    case MOTOR_AVANCE:
        HAL_GPIO_WritePin(MI0_GPIO_Port, MI0_Pin, GPIO_PIN_SET);   // MI0 = 1
        HAL_GPIO_WritePin(MI1_GPIO_Port, MI1_Pin, GPIO_PIN_RESET); // MI1 = 0
        break;

    case MOTOR_RETROCESO:
        HAL_GPIO_WritePin(MI0_GPIO_Port, MI0_Pin, GPIO_PIN_RESET); // MI0 = 0
        HAL_GPIO_WritePin(MI1_GPIO_Port, MI1_Pin, GPIO_PIN_SET);   // MI1 = 1
        break;

    case MOTOR_FRENADO:
    default:
        HAL_GPIO_WritePin(MI0_GPIO_Port, MI0_Pin, GPIO_PIN_RESET); // MI0 = 0
        HAL_GPIO_WritePin(MI1_GPIO_Port, MI1_Pin, GPIO_PIN_RESET); // MI1 = 0
        pwm = 0;                                                   // Forzar PWM a 0 en frenado
        break;
    }

    // Establecer PWM, aca le definimos la velocidad
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pwm);
}

/**
 * @brief Configura motor derecho
 * @param estado: MOTOR_AVANCE, MOTOR_RETROCESO o MOTOR_FRENADO
 * @param pwm: Valor PWM (0-1000)
 */
void set_motor_der(motor_estado_t estado, uint16_t pwm)
{
    switch (estado)
    {
    case MOTOR_AVANCE:
        HAL_GPIO_WritePin(MD0_GPIO_Port, MD0_Pin, GPIO_PIN_SET);   // MD0 = 1
        HAL_GPIO_WritePin(MD1_GPIO_Port, MD1_Pin, GPIO_PIN_RESET); // MD1 = 0
        break;

    case MOTOR_RETROCESO:
        HAL_GPIO_WritePin(MD0_GPIO_Port, MD0_Pin, GPIO_PIN_RESET); // MD0 = 0
        HAL_GPIO_WritePin(MD1_GPIO_Port, MD1_Pin, GPIO_PIN_SET);   // MD1 = 1
        break;

    case MOTOR_FRENADO:
    default:
        HAL_GPIO_WritePin(MD0_GPIO_Port, MD0_Pin, GPIO_PIN_RESET); // MD0 = 0
        HAL_GPIO_WritePin(MD1_GPIO_Port, MD1_Pin, GPIO_PIN_RESET); // MD1 = 0
        pwm = 0;                                                   // Forzar PWM a 0 en frenado
        break;
    }

    // Establecer VELOCIDAD
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pwm);
}

/**
 * @brief Avanza con ambos motores al 70% de velocidad
 */
void avanza(void)
{
    set_motor_izq(MOTOR_AVANCE, velocidad_actual_izq);
    set_motor_der(MOTOR_AVANCE, velocidad_actual_der);
}

/**
 * @brief Gira 90 grados a la izquierda y luego continúa avanzando
 * Motor izq retrocede, motor der avanza al 100%
 */
brujula gira90izq(brujula sentido)
{
    set_motor_izq(MOTOR_RETROCESO, velocidad_giro_actual_izq);
    set_motor_der(MOTOR_AVANCE, velocidad_giro_actual_der);

    HAL_Delay(TIEMPO_GIRO_90_IZQ);
    switch (sentido)
    {
    case norte:
        sentido = oeste;
        break;

    case este:
        sentido = norte;
        break;

    case sur:
        sentido = este;
        break;

    case oeste:
        sentido = sur;
        break;
    }

    // Después del giro, continuar avanzando
    //avanza();

    // termina de girar y verifica que no halla un muro adelante

    return sentido;
}

/**
 * @brief Gira 90 grados a la derecha y luego continúa avanzando
 * Motor der retrocede, motor izq avanza al 100%
 */
brujula gira90der(brujula sentido)
{
    set_motor_izq(MOTOR_AVANCE, velocidad_giro_actual_izq);
    set_motor_der(MOTOR_RETROCESO, velocidad_giro_actual_der);

    HAL_Delay(TIEMPO_GIRO_90_DER);
    switch (sentido)
    {
    case norte:
        sentido = este;
        break;

    case este:
        sentido = sur;
        break;

    case sur:
        sentido = oeste;
        break;

    case oeste:
        sentido = norte;
        break;
    }

    // Después del giro, continuar avanzando
    //avanza();
    
    return sentido;
}

/**
 * @brief Gira 180 grados y luego continúa avanzando
 * Motor der retrocede, motor izq avanza al 100%
 */
brujula gira180(brujula sentido)
{
    set_motor_izq(MOTOR_AVANCE, velocidad_giro_actual_izq);
    set_motor_der(MOTOR_RETROCESO, velocidad_giro_actual_der);

    HAL_Delay(TIEMPO_GIRO_180);
    switch (sentido)
    {
    case norte:
        sentido = sur;
        break;

    case este:
        sentido = oeste;
        break;

    case sur:
        sentido = norte;
        break;

    case oeste:
        sentido = este;
        break;
    }

    // Después del giro, continuar avanzando
    //avanza();
    return sentido;
}

/**
 * @brief Detiene ambos motores (cuando ganemos)
 */
void termino(void)
{
    set_motor_izq(MOTOR_FRENADO, 0);
    set_motor_der(MOTOR_FRENADO, 0);
}

void correccion_izquierda(void)
{

    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 100); // Motor izq más lento
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 700); // Motor der normal
     for (int i = 0; i < 7; i++)                       // 10 ciclos de 10 ms = 100 ms de corrección
    {
        if (flag_linea_detectada || flag_muro_detectado)
            return; // Salir si hay algo urgente

        HAL_Delay(10);
    } 
}

void correccion_derecha(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 700); // Motor izq normal
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 100); // Motor der más lento
     for (int i = 0; i < 7; i++)                       // 10 ciclos de 10 ms = 100 ms de corrección
    {
        if (flag_linea_detectada || flag_muro_detectado)
            return; // Salir si hay algo urgente

        HAL_Delay(10);
    } 
}
