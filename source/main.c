/*
 * main.c
 *
 *  Created on: Dec 19, 2024
 *      Author: hnaco
 */

#include "MKE16Z4.h"
#include "uart.h"
#include "clock.h"
#include "port.h"
#include <stdarg.h>
#include <stdio.h>

LPUART_InitStruct_t LPUART_InitStruct = {
	.BaudRate = 115200,
	.NumberData = 8,
	.ParityConfig = LPUART_PARITY_NONE,
	.MSB_First = 0,
	.PolarityConfig = {
		.LPUART_TxPolarity = 0,
		.LPUART_RxPolarity = 0},
	.StopBitConfig = LPUART_STOPBITNUMBER_1,
	.Frequency = 48000000,
	.EnableRxInterrupt = 1};

PinConfigType config = {
	.Mux = PORT_MUX_ALT2,
	.Interrupt = 0,
	.CallBack = 0,
};

void UART_ISR(void)
{
	char data;
	UART_ReadByte(LPUART0, &data);
	UART_WriteByte(LPUART0, data);
}

void Serial_Printf(char *format, ...)
{
	char buffer[100];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	for(int i = 0; buffer[i] != '\0'; i++)
	{
		UART_WriteByte(LPUART0, buffer[i]);
	}
}

int main()
{
	/* Enable clock for PORTB */
	EnableClockForPort(PCC_PORTB_INDEX);
	PORT_Pin_Init(PORT_INS_B, 0, &config);
	PORT_Pin_Init(PORT_INS_B, 1, &config);
	UART_Init(LPUART0, &LPUART_InitStruct);
	UART_RegisterHandler(LPUART0, UART_ISR);
	Serial_Printf("Hello World\n");
	while (1)
	{
	}
}
