/*
 * serial.h
 *
 *  Created on: Dec 25, 2024
 *      Author: nvd
 */

#ifndef SERIAL_H_
#define SERIAL_H_
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include "my_lpuart.h"
#include "my_circular_queue.h"
#include "my_clock.h"
void UART_ISR(void);
void Serial_begin(LPUART_Type *base, uint32_t baudRate);
void Serial_print(LPUART_Type *base, char *message);
void Serial_sendByte(LPUART_Type *base, uint8_t data);
void Serial_println(LPUART_Type *base, char *message);
void Serial_writeByte(LPUART_Type *base, uint8_t data);
bool Serial_available(LPUART_Type *base,uint8_t number);
void Serial_Printf(LPUART_Type *base,char *format, ...);
char *Serial_readBytes(LPUART_Type *base);
char* Serial_readStringUntil(LPUART_Type *base, char delimiter);
#endif /* SERIAL_H_ */
