/*
 * uart.h
 *
 *  Created on: Jun 9, 2025
 *      Author: tamis
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h>
#include "main.h"

extern char mensaje[16];
extern const uint8_t delay;
extern uint8_t buffer[16];
extern UART_HandleTypeDef huart5;

void Transmision(void);
void Inicializar_UART(void);

#endif /* INC_UART_H_ */