/*
 * uart.h
 *
 *  Created on: Dec 19, 2024
 *      Author: hnaco
 */

#ifndef MY_UART_H_
#define MY_UART_H_

#include "MKE16Z4.h"
#include "my_clock.h"
#include "my_port.h"


typedef struct
{
    uint8_t LPUART_EnableTx;
    uint8_t LPUART_EnableRx;
} LPUART_Enable_Struct_t;

typedef enum
{
    LPUART_PARITY_NONE = 0U,
    LPUART_PARITY_EVEN = 1U,
    LPUART_PARITY_ODD = 2U
} LPUART_Parity_t;

typedef enum
{
    LPUART_STOPBITNUMBER_1 = 0U,
    LPUART_STOPBITNUMBER_2 = 1U,
} LPUART_StopBitNumber_t;

typedef struct
{
    uint8_t LPUART_TxPolarity;
    uint8_t LPUART_RxPolarity;
} LPUART_DataPolarity_Struct_t;



typedef struct
{
    int BaudRate;
    uint8_t NumberData;
    LPUART_Parity_t ParityConfig;
    uint8_t MSB_First;
    LPUART_DataPolarity_Struct_t PolarityConfig;
    LPUART_StopBitNumber_t StopBitConfig;
    uint8_t EnableRxInterrupt;

} LPUART_InitStruct_t;

typedef void (*LPUART_Handler_t)(void);

void UART_Init(LPUART_Type *base, LPUART_InitStruct_t *LPUART_InitStruct);
void UART_WriteByte(LPUART_Type *base, uint8_t data);
void UART_Write(LPUART_Type *base, uint8_t *data, uint32_t size);
void UART_ReadByte(LPUART_Type *base, uint8_t *data);
void UART_RegisterHandler(LPUART_Type *base, LPUART_Handler_t handler);

#endif /* MY_UART_H_ */
