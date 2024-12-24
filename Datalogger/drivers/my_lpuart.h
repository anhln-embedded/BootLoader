/*
 * my_uart.h
 *
 *  Created on: Dec 21, 2024
 *      Author: nvd
 */
// PTB1 -2-TX lpuart 0
// PTB0 -2-RX lpuart 0

// PTC7 -2-TX lpuart 1
// PTC6 -2-RX lpuart 1

// PTD7 -2-TX lpuart 2
// PTD6 -2-RX lpuart 2

#ifndef MY_UART_H_
#define MY_UART_H_
#include "MKE16Z4.h"
#include <stdint.h>
#include <stdlib.h>
// Function prototypes
void LPUART_Callback();
void LPUART_begin(LPUART_Type *base, uint32_t baudRate);
void LPUART_enableInterrupts();
void LPUART_print(char *data);
void LPUART_println(char *data);

uint8_t LPUART_available();
char *LPUART_readString(uint32_t size);
char *LPUART_readUntil(char endChar);
uint32_t LPUART_readBytes(uint8_t *buffer, uint32_t size);

void enableLPUARTClock(void);
// Configure the pins for UART operation (TX and RX pins)
void setupPinPort(void);

#endif /* MY_UART_H_ */
