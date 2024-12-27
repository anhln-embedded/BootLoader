/*
 * uart.c
 *
 *  Created on: Dec 19, 2024
 *      Author: hnaco
 */

#include "uart.h"
#include <math.h>

LPUART_Handler_t LPUART0_RegisterHandler = 0;
LPUART_Handler_t LPUART1_RegisterHandler = 0;
LPUART_Handler_t LPUART2_RegisterHandler = 0;

static void UART_SetNumberData(LPUART_Type *base, uint8_t numberData)
{
	switch (numberData)
	{
	case 7:
	{
		base->BAUD &= ~LPUART_BAUD_M10_MASK;
		base->CTRL |= LPUART_CTRL_M7_MASK;
		break;
	}
	case 8:
	{
		base->BAUD &= ~LPUART_BAUD_M10_MASK;
		base->CTRL &= ~LPUART_CTRL_M7_MASK;
		base->CTRL &= ~LPUART_CTRL_M_MASK;
		break;
	}
	case 9:
	{
		base->BAUD &= ~LPUART_BAUD_M10_MASK;
		base->CTRL &= ~LPUART_CTRL_M7_MASK;
		base->CTRL |= LPUART_CTRL_M_MASK;
		break;
	}
	case 10:
	{
		base->BAUD |= LPUART_BAUD_M10_MASK;
		base->CTRL &= ~LPUART_CTRL_M7_MASK;
		break;
	}
	default:
		break;
	}
}

static void UART_SetParity(LPUART_Type *base, LPUART_Parity_t parityConfig)
{
    switch (parityConfig)
    {
        case LPUART_PARITY_NONE:
        {
            base->CTRL &= ~LPUART_CTRL_PE_MASK;
            break;
        }
        case LPUART_PARITY_EVEN:
        {
            base->CTRL |= LPUART_CTRL_PE_MASK;
            base->CTRL &= ~LPUART_CTRL_PT_MASK;
            break;
        }
        case LPUART_PARITY_ODD:
        {
            base->CTRL |= LPUART_CTRL_PE_MASK;
            base->CTRL |= LPUART_CTRL_PT_MASK;
            break;
        }
        default:
            break;
    }	
}

static void UART_SetBaudrate(LPUART_Type *base, uint32_t baudrate, uint32_t frequency)
{
    float diver = (float)frequency / baudrate;
    uint8_t osr;
    uint8_t osr_temp;
    uint16_t sbr_temp;
    float min_delta = baudrate;

    for (osr = 4; osr <= 32; osr++)
    {
        sbr_temp = (uint16_t)(diver / osr);
        if ((sbr_temp >= 1) && (sbr_temp <= 8191))
        {
            if (fabs(sbr_temp * osr - diver) <= min_delta)
            {
                osr_temp = osr;
            }
        }
    }
    if ((osr_temp >= 4) || (osr_temp <= 7))
    {
        base->BAUD |= LPUART_BAUD_BOTHEDGE_MASK;
    }
    base->BAUD = (base->BAUD & ~LPUART_BAUD_OSR_MASK) | LPUART_BAUD_OSR(osr_temp - 1);
    base->BAUD = (base->BAUD & ~LPUART_BAUD_SBR_MASK) | LPUART_BAUD_SBR(sbr_temp);
}

static void HAL_LPUART_EnableRxInterrupt(LPUART_Type *base)
{
    base->CTRL |= LPUART_CTRL_RIE_MASK;
	if(base == LPUART0)
	{
		NVIC_EnableIRQ(LPUART0_IRQn);
	}
	else if(base == LPUART1)
	{
		NVIC_EnableIRQ(LPUART1_IRQn);
	}
	else if(base == LPUART2)
	{
		NVIC_EnableIRQ(LPUART2_IRQn);
	}
}

void UART_Init(LPUART_Type *base, LPUART_InitStruct_t *LPUART_InitStruct)
{
	if (base == LPUART0)
	{
		SCG->FIRCDIV = (SCG->FIRCDIV & ~SCG_FIRCDIV_FIRCDIV2_MASK) | SCG_FIRCDIV_FIRCDIV2(1);
		PCC->CLKCFG[PCC_LPUART0_INDEX] = (PCC->CLKCFG[PCC_LPUART0_INDEX] & ~PCC_CLKCFG_PCS_MASK) | PCC_CLKCFG_PCS(3);
		PCC->CLKCFG[PCC_LPUART0_INDEX] |= PCC_CLKCFG_CGC(1);
	}
	else if (base == LPUART1)
	{
		SCG->FIRCDIV = (SCG->FIRCDIV & ~SCG_FIRCDIV_FIRCDIV2_MASK) | SCG_FIRCDIV_FIRCDIV2(1);
		PCC->CLKCFG[PCC_LPUART1_INDEX] = (PCC->CLKCFG[PCC_LPUART1_INDEX] & ~PCC_CLKCFG_PCS_MASK) | PCC_CLKCFG_PCS(3);
		PCC->CLKCFG[PCC_LPUART1_INDEX] |= PCC_CLKCFG_CGC(1);
	}
	else if (base == LPUART2)
	{
		SCG->FIRCDIV = (SCG->FIRCDIV & ~SCG_FIRCDIV_FIRCDIV2_MASK) | SCG_FIRCDIV_FIRCDIV2(1);
		PCC->CLKCFG[PCC_LPUART2_INDEX] = (PCC->CLKCFG[PCC_LPUART2_INDEX] & ~PCC_CLKCFG_PCS_MASK) | PCC_CLKCFG_PCS(3);
		PCC->CLKCFG[PCC_LPUART2_INDEX] |= PCC_CLKCFG_CGC(1);
	}

	/*Reset module*/
	base->GLOBAL |= LPUART_GLOBAL_RST_MASK;
	base->GLOBAL &= ~LPUART_GLOBAL_RST_MASK;

	/*Disable Tx Rx*/
	base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
	/*Mode Select*/
	UART_SetNumberData(base, LPUART_InitStruct->NumberData);
	/*Disable parity*/
	UART_SetParity(base, LPUART_InitStruct->ParityConfig);
	/*Set number stop bit*/
	base->BAUD =  (base->BAUD & ~LPUART_BAUD_SBNS_MASK) | LPUART_BAUD_SBNS(LPUART_InitStruct->StopBitConfig);

	/*Set baudrate*/
	UART_SetBaudrate(base, LPUART_InitStruct->BaudRate, LPUART_InitStruct->Frequency);

	/*Enable rx interrupt*/
	if (LPUART_InitStruct->EnableRxInterrupt)
	{
		HAL_LPUART_EnableRxInterrupt(base);
	}
	/*Enable Tx Rx*/
	base->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

}
void UART_WriteByte(LPUART_Type *base, uint8_t data)
{
    base->DATA |= data;
    while ((base->STAT & LPUART_STAT_TC_MASK) == 0);	
}
void UART_Write(LPUART_Type *base, uint8_t *data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		UART_WriteByte(base, data[i]);
	}
}
void UART_ReadByte(LPUART_Type *base, uint8_t *data)
{
	*data = base->DATA;
}

void UART_RegisterHandler(LPUART_Type *base, LPUART_Handler_t handler)
{
	if (base == LPUART0)
	{
		LPUART0_RegisterHandler = handler;
	}
	else if (base == LPUART1)
	{
		LPUART1_RegisterHandler = handler;
	}
	else if (base == LPUART2)
	{
		LPUART2_RegisterHandler = handler;
	}
}

void LPUART0_IRQHandler(void)
{
	if (LPUART0_RegisterHandler)
	{
		LPUART0_RegisterHandler();
	}
}
void LPUART1_IRQHandler(void)
{
	if (LPUART1_RegisterHandler)
	{
		LPUART1_RegisterHandler();
	}
}
void LPUART2_IRQHandler(void)
{
	if (LPUART2_RegisterHandler)
	{
		LPUART2_RegisterHandler();
	}
}
