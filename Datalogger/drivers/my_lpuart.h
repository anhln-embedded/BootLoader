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

typedef void (*LPUART_CallBackType)(void);

void LPUART_begin(LPUART_Type *base, uint32_t baudRate);
void LPUART_enableRxInterrupts(LPUART_Type *base, LPUART_CallBackType callback);
void LPUART_write(LPUART_Type *base, uint8_t data);
uint8_t LPUART_available(LPUART_Type *base);
uint8_t LPUART_readByte(LPUART_Type *base);

#endif /* MY_UART_H_ */
