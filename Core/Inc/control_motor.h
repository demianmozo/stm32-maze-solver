/**
 * @file control_motor.h
 * @brief Módulo de control de motores
 * @author demianmozo
 * @date 2025-06-07
 * @version 1.0
 *
 * Este módulo controla los dos motores del robot usando:
 * - Pines digitales para dirección (MI0/MI1, MD0/MD1)
 * - PWM para velocidad (PC8, PC9)
 * - Timer TIM3 para generar señales PWM
 */

#ifndef __CONTROL_MOTOR_H
#define __CONTROL_MOTOR_H

#include "main.h"

extern uint16_t velocidad_actual_izq;
extern uint16_t velocidad_actual_der;
extern uint16_t velocidad_giro_actual_izq;
extern uint16_t velocidad_giro_actual_der;

/* Definiciones para control de motores */
// #define VELOCIDAD_AVANCE 700 // 70% de 1000 (período del timer)

void activar_modo_sprint(void);  // Declaración
#define VELOCIDAD_AVANCE_IZQ 700 // Motor izquierdo avance
#define VELOCIDAD_AVANCE_DER 700 // Motor derecho avance
#define VELOCIDAD_SPRINT_IZQ 900 // 90% - Motor izq modo velocidad máxima
#define VELOCIDAD_SPRINT_DER 900 // 90% - Motor der modo velocidad máxima
#define VELOCIDAD_GIRO_IZQ 700   // Motor izquierdo giro a la izquierda
#define VELOCIDAD_GIRO_DER 700   // Motor derecho giro a la derecha

/* Tiempos de giro en milisegundos (ajustar según calibración) */
#define TIEMPO_GIRO_90_IZQ 500  // Tiempo para giro de 90 grados a la izquierda
#define TIEMPO_GIRO_90_DER 550  // Tiempo para giro de 90 grados a la derecha

#define TIEMPO_GIRO_180 1100 // Tiempo para giro de 180 grados

/**
 * @brief Estados de motor según tabla de control
 *
 * | M1 | M0 | Acción    |
 * |----|----|-----------|
 * | 0  | 1  | Avance    |
 * | 1  | 0  | Retroceso |
 * | 0  | 0  | Frenado   |
 * | 1  | 1  | Frenado   |
 */

typedef enum // Un enum es un tipo de dato que define un conjunto de
// constantes con nombres en lugar de usar números directamente.
// para este caso, avance=0, retroceso=1, frenado=2
// no se usan los nombres directamente xq chat dice que es una cagada usar strings en un micro, y no me compilaba creo que por eso
{
    MOTOR_AVANCE = 0, // M1=0, M0=1
    MOTOR_RETROCESO,  // M1=1, M0=0
    MOTOR_FRENADO     // M1=0, M0=0 o M1=1, M0=1
} motor_estado_t;

typedef enum // enumeracion para variable de brujula
{
    norte = 0,
    este,
    sur,
    oeste
} brujula;

/**
 * @defgroup ControlMotor Control de Motores
 * @brief Funciones para navegación del robot en el laberinto
 * @{
 */

/**
 * @brief Inicializa el sistema de control de motores
 * @details Configura PWM en TIM3 canales 3 y 4, y detiene motores
 * @note Debe llamarse después de HAL_Init() y configuración de GPIO
 */
void control_motor_init(void);

/**
 * @brief Avanza con ambos motores al 70% de velocidad
 * @details Configura ambos motores en modo avance con VELOCIDAD_AVANCE
 */
void avanza(void);

/**
 * @brief Gira 90 grados a la izquierda y continúa avanzando
 * @details Motor izquierdo retrocede, motor derecho avanza, al 100%
 */
brujula gira90izq(brujula sentido);

/**
 * @brief Gira 90 grados a la derecha y continúa avanzando
 * @details Motor derecho retrocede, motor izquierdo avanza, al 100%
 */
brujula gira90der(brujula sentido);

/**
 * @brief Gira 180 grados y continúa avanzando
 * @details Motor derecho retrocede, motor izquierdo avanza, al 100%
 */
brujula gira180(brujula sentido);

/**
 * @brief Detiene ambos motores completamente
 * @details Usado cuando el robot gana
 */
void termino(void);

/** @} */ // fin grupo ControlMotor

/**
 * @defgroup ControlMotorAux Funciones Auxiliares de Control
 * @brief Funciones de bajo nivel para control individual de motores
 * @{
 */

/**
 * @brief Configura el motor izquierdo
 * @param estado Estado del motor (MOTOR_AVANCE, MOTOR_RETROCESO, MOTOR_FRENADO)
 * @param pwm Valor PWM de velocidad (0-1000)
 * @details Controla pines MI0/MI1 para dirección y Vel_I para velocidad
 */
void set_motor_izq(motor_estado_t estado, uint16_t pwm);

/**
 * @brief Configura el motor derecho
 * @param estado Estado del motor (MOTOR_AVANCE, MOTOR_RETROCESO, MOTOR_FRENADO)
 * @param pwm Valor PWM de velocidad (0-1000)
 * @details Controla pines MD0/MD1 para dirección y Vel_D para velocidad
 */
void set_motor_der(motor_estado_t estado, uint16_t pwm);

void correccion_izquierda(void);
void correccion_derecha(void);

/** @} */ // fin grupo ControlMotorAux

#endif /* __CONTROL_MOTOR_H */
