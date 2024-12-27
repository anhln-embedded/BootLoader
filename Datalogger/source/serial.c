/*
 * serial.c
 *
 *  Created on: Dec 25, 2024
 *      Author: nvd
 */

<<<<<<< HEAD
#include <my_uart.h>
#include "serial.h"
#include "my_circular_queue.h"
=======
#include "serial.h"
#include "my_circular_queue.h"

>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
// Static variables
static CircularQueue_t queue;
static LPUART_Type *g_base;

// UART Interrupt Service Routine
void UART_ISR(void)
{
<<<<<<< HEAD
    uint8_t data;
    UART_ReadByte(g_base, &data);
=======
    while (!LPUART_available(g_base));
    uint8_t data = LPUART_readByte(g_base);
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
    CircularQueue_Enqueue(&queue, data);
}

// Initialize the UART with the specified baud rate
void Serial_begin(LPUART_Type *base, uint32_t baudRate)
{
    g_base = base;
    CircularQueue_Init(&queue);

<<<<<<< HEAD
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
=======
    LPUART_begin(g_base, baudRate);
    LPUART_enableRxInterrupts(g_base, UART_ISR);
}

// Check if data is available to read from UART
bool Serial_available(LPUART_Type *base,uint8_t number)
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
{

    return CircularQueue_Size(&queue)>=number;
}

<<<<<<< HEAD
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
=======
// Send a string via UART
void Serial_print(LPUART_Type *base, char *message)
{
    uint32_t i = 0;
    while (message[i] != '\0') {
        LPUART_write(base, message[i]);
        i++;
    }
}

// Send a string followed by a newline via UART
void Serial_println(LPUART_Type *base, char *message)
{
    Serial_print(base, message);   // Print the message
    Serial_print(base, "\r\n");    // Add carriage return and newline
}

// Write a single byte of data via UART
void Serial_writeByte(LPUART_Type *base, uint8_t data)
{
    LPUART_write(base, data);  // Transmit the byte
}

// Write a formatted string via UART
void Serial_Printf(LPUART_Type *base,char *format, ...)
{
    char buffer[100];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    for (int i = 0; buffer[i] != '\0'; i++) {
        Serial_writeByte(base, buffer[i]);
    }
}

// Read a byte of data from UART
char *Serial_readBytes(LPUART_Type *base)
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
{
    static char data[128];  // Static buffer to store the received bytes
    uint8_t i = 0;  // Index for storing bytes
    uint8_t isEmpty = CircularQueue_IsEmpty(&queue);
    while (!isEmpty && i < 127) {  // Avoid buffer overflow
        CircularQueue_Dequeue(&queue, (uint8_t *)&data[i]);  // Store byte in data
        i++;
        isEmpty = CircularQueue_IsEmpty(&queue);
    }
<<<<<<< HEAD
    data[i] = '\0';
    return data;
}

=======
    data[i] = '\0';  // Null-terminate the string after the last byte
    return data;     // Return the buffer containing received bytes
}

// Read a string from UART until a delimiter is found
char* Serial_readStringUntil(LPUART_Type *base, char delimiter)
{
    char *buffer = NULL;
    size_t length = 0;

    while (1) {
        uint8_t data = LPUART_readByte(base);
        char *newBuffer = realloc(buffer, length + 2);
        if (newBuffer == NULL) {
            free(buffer);
            return NULL;
        }
        buffer = newBuffer;
        buffer[length++] = data;
        if (data == delimiter) {
            break;
        }
    }
    buffer[length] = '\0';
    return buffer;
}
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
