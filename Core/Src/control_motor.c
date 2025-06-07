#include "control_motor.h"

extern TIM_HandleTypeDef htim3; // usa el timer 3 para PWM

/**
 * @brief Inicializa el control de motores
 */
void control_motor_init(void)
{
    // Iniciar PWM en ambos canales
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // Motor izquierdo (PC8)
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); // Motor derecho (PC9)

    // Asegurar que motores estén detenidos al inicio
    termino();
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
        HAL_GPIO_WritePin(GPIOB, MI0_Pin, GPIO_PIN_SET);   // MI0 = 1
        HAL_GPIO_WritePin(GPIOB, MI1_Pin, GPIO_PIN_RESET); // MI1 = 0
        break;

    case MOTOR_RETROCESO:
        HAL_GPIO_WritePin(GPIOB, MI0_Pin, GPIO_PIN_RESET); // MI0 = 0
        HAL_GPIO_WritePin(GPIOB, MI1_Pin, GPIO_PIN_SET);   // MI1 = 1
        break;

    case MOTOR_FRENADO:
    default:
        HAL_GPIO_WritePin(GPIOB, MI0_Pin, GPIO_PIN_RESET); // MI0 = 0
        HAL_GPIO_WritePin(GPIOB, MI1_Pin, GPIO_PIN_RESET); // MI1 = 0
        pwm = 0;                                           // Forzar PWM a 0 en frenado
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
        HAL_GPIO_WritePin(GPIOB, MD0_Pin, GPIO_PIN_SET);   // MD0 = 1
        HAL_GPIO_WritePin(GPIOB, MD1_Pin, GPIO_PIN_RESET); // MD1 = 0
        break;

    case MOTOR_RETROCESO:
        HAL_GPIO_WritePin(GPIOB, MD0_Pin, GPIO_PIN_RESET); // MD0 = 0
        HAL_GPIO_WritePin(GPIOB, MD1_Pin, GPIO_PIN_SET);   // MD1 = 1
        break;

    case MOTOR_FRENADO:
    default:
        HAL_GPIO_WritePin(GPIOB, MD0_Pin, GPIO_PIN_RESET); // MD0 = 0
        HAL_GPIO_WritePin(GPIOB, MD1_Pin, GPIO_PIN_RESET); // MD1 = 0
        pwm = 0;                                           // Forzar PWM a 0 en frenado
        break;
    }

    // Establecer PWM
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pwm);
}

/**
 * @brief Avanza con ambos motores al 70% de velocidad
 */
void avanza(void)
{
    set_motor_izq(MOTOR_AVANCE, VELOCIDAD_AVANCE);
    set_motor_der(MOTOR_AVANCE, VELOCIDAD_AVANCE);
}

/**
 * @brief Gira 90 grados a la izquierda y luego continúa avanzando
 * Motor izq retrocede, motor der avanza al 100%
 */
void gira90izq(void)
{
    set_motor_izq(MOTOR_RETROCESO, VELOCIDAD_GIRO);
    set_motor_der(MOTOR_AVANCE, VELOCIDAD_GIRO);

    HAL_Delay(TIEMPO_GIRO_90);

    // Después del giro, continuar avanzando
    avanza();
}

/**
 * @brief Gira 90 grados a la derecha y luego continúa avanzando
 * Motor der retrocede, motor izq avanza al 100%
 */
void gira90der(void)
{
    set_motor_izq(MOTOR_AVANCE, VELOCIDAD_GIRO);
    set_motor_der(MOTOR_RETROCESO, VELOCIDAD_GIRO);

    HAL_Delay(TIEMPO_GIRO_90);

    // Después del giro, continuar avanzando
    avanza();
}

/**
 * @brief Gira 180 grados y luego continúa avanzando
 * Motor der retrocede, motor izq avanza al 100%
 */
void gira180(void)
{
    set_motor_izq(MOTOR_AVANCE, VELOCIDAD_GIRO);
    set_motor_der(MOTOR_RETROCESO, VELOCIDAD_GIRO);

    HAL_Delay(TIEMPO_GIRO_180);

    // Después del giro, continuar avanzando
    avanza();
}

/**
 * @brief Detiene ambos motores (cuando ganemos)
 */
void termino(void)
{
    set_motor_izq(MOTOR_FRENADO, 0);
    set_motor_der(MOTOR_FRENADO, 0);
}