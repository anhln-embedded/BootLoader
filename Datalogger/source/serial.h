/*
 * serial.h
 *
 *  Created on: Dec 25, 2024
 *      Author: nvd
 */

#ifndef SERIAL_H_
#define SERIAL_H_
<<<<<<< HEAD
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
=======
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
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
#endif /* SERIAL_H_ */
