/*
 * serial.h
 *
 *  Created on: Dec 25, 2024
 *      Author: nvd
 */

#ifndef SERIAL_H_
#define SERIAL_H_
#include <my_uart.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include "my_circular_queue.h"
#include "my_clock.h"
// UART Interrupt Service Routine
void UART_ISR(void);

// Initialize the UART with the specified baud rate
void Serial_begin(LPUART_Type *base, uint32_t baudRate);

// Check if data is available to read from UART
bool Serial_available(uint8_t number);

// Write a formatted string via UART
void Serial_printf(char *format, ...);

void Serial_print(char *str);
// Read a byte of data from UART
char *Serial_readBytes(void);

void Serial_clear();
#endif /* SERIAL_H_ */
