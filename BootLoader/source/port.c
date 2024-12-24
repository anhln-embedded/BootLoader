/*
 * port.c
 *
 *  Created on: Nov 26, 2024
 *      Author: hnaco
 */

#include "port.h"
#include "string.h"

/*******************************************************************************
* Variables
******************************************************************************/

static CallBackType s_PORTBCD_Handler = NULL;
static CallBackType s_PORTAE_Handler = NULL;

/*******************************************************************************
* Code
******************************************************************************/

status_t PORT_Pin_Init(
    Port_Instance_t portInstance,
    uint8_t pin,
    const PinConfigType *pPinConfig)
{
    status_t retVal = STATUS_OK;
    retVal &= (0 <= pin && pin <= 31);
    retVal &= (pPinConfig != 0);
    retVal &= (portInstance < PORT_INS_NUM);
    if (STATUS_OK == retVal)
    {
        PORT_Type *PORTx;
        PORTx = s_PortInstance[portInstance];
        PORTx->PCR[pin] = 0;

        PORTx->PCR[pin] |= PORT_PCR_MUX(pPinConfig->Mux);
        PORTx->PCR[pin] |= pPinConfig->Pull << 0;
        if (pPinConfig->Interrupt != PORT_INTERRUPT_DISABLE)
        {
            PORTx->PCR[pin] |= PORT_PCR_IRQC(pPinConfig->Interrupt);
            if (PORTx == PORTA || PORTx == PORTE)
            {
                s_PORTAE_Handler = pPinConfig->CallBack;
            }

            if (PORTx == PORTB || PORTx == PORTC || PORTx == PORTD)
            {
                NVIC->ISER[0] |= (1 << 26);
                s_PORTBCD_Handler = pPinConfig->CallBack;
            }
        }
    }

    return retVal;
}

void PORTBCD_IRQHandler(void)
{
    if (PORTD->ISFR)
    {
        for (int pin = 0; pin < 32; pin++)
        {
            if (PORTD->ISFR & (1 << pin))
            {
                PORTD->ISFR |= (1 << pin);
                if (s_PORTBCD_Handler)
                {
                    s_PORTBCD_Handler(pin);
                }
            }
        }
    }
}

void PORTAE_IRQHandler(void)
{
    // Implement handler if needed
}