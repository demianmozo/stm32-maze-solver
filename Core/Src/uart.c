/*
 * uart.c
 *
 *  Created on: Jun 9, 2025
 *      Author: tamis
 */

#include "main.h"
#include "uart.h"
#include <stdio.h> //Nos permite usar la función sprintf
#include <string.h> //Nos permite usar la función strcat
#include <stdint.h>

char mensaje[16];
const uint8_t delay = 50;
uint8_t buffer[16];

void Transmision(void) {
    strcat(mensaje, "\r\n");
    HAL_UART_Transmit(&huart5, (uint8_t*) mensaje, strlen(mensaje), delay);
}

void Inicializar_UART(void) {
    HAL_UART_Receive_IT(&huart5, buffer, sizeof(buffer));
    mensaje[0]='\r';
    mensaje[1]='\n';
    mensaje[2]='\0';
    Transmision();
    strcpy(mensaje, "UART conectada");
    Transmision();
    mensaje[0]='\0';

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

