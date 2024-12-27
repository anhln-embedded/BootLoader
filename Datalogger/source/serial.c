/*
 * serial.c
 *
 *  Created on: Dec 25, 2024
 *      Author: nvd
 */

#include <my_uart.h>
#include "serial.h"
#include "my_circular_queue.h"
// Static variables
static CircularQueue_t queue;
static LPUART_Type *g_base;

// UART Interrupt Service Routine
void UART_ISR(void)
{
    uint8_t data;
    UART_ReadByte(g_base, &data);
    CircularQueue_Enqueue(&queue, data);
}

// Initialize the UART with the specified baud rate
void Serial_begin(LPUART_Type *base, uint32_t baudRate)
{
    g_base = base;
    CircularQueue_Init(&queue);

    LPUART_InitStruct_t LPUART_InitStruct = {
        .BaudRate = baudRate,
        .NumberData = 8,
        .ParityConfig = LPUART_PARITY_NONE,
        .MSB_First = 0,
        .PolarityConfig = {
            .LPUART_TxPolarity = 0,
            .LPUART_RxPolarity = 0},
        .StopBitConfig = LPUART_STOPBITNUMBER_1,
        .EnableRxInterrupt = 1};
    UART_Init(base, &LPUART_InitStruct);
    UART_RegisterHandler(base, UART_ISR);
}

// Check if data is available to read from UART
bool Serial_available(uint8_t number)
{

    return CircularQueue_Size(&queue)>=number;
}

void Serial_clear(){
	CircularQueue_Free(&queue);
}
// Write a formatted string via UART
void Serial_Printf(char *format, ...)
{
	    char buffer[100];
	    va_list args;
	    va_start(args, format);
	    vsnprintf(buffer, sizeof(buffer), format, args);
	    va_end(args);
	    for (int i = 0; buffer[i] != '\0'; i++)
	    {
	        UART_WriteByte(g_base, buffer[i]);
	    }
}
void Serial_print(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        UART_WriteByte(g_base, str[i]);
    }
}
// Read a byte of data from UART
char *Serial_readBytes()
{
    static char data[128];  // Static buffer to store the received bytes
    uint8_t i = 0;  // Index for storing bytes
    uint8_t isEmpty = CircularQueue_IsEmpty(&queue);
    while (!isEmpty && i < 127) {  // Avoid buffer overflow
        CircularQueue_Dequeue(&queue, (uint8_t *)&data[i]);  // Store byte in data
        i++;
        isEmpty = CircularQueue_IsEmpty(&queue);
    }
    data[i] = '\0';
    return data;
}

