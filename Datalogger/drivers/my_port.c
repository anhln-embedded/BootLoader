/*
 * port.c
 *
 *  Created on: Nov 26, 2024
 *      Author: hnaco
 */

#include <my_port.h>
#include "string.h"

/*******************************************************************************
* Variables
******************************************************************************/

static CallBackType s_PORT_Handler = NULL;

/*******************************************************************************
* Code
******************************************************************************/

status_t PORT_Pin_Init(
		PORT_Type * PORTx,
    uint8_t pin,
    const PortConfig_t *pPinConfig)
{
    status_t retVal = STATUS_OK;
    retVal &= (0 <= pin && pin <= 31);
    retVal &= (pPinConfig != 0);
    if (STATUS_OK == retVal)
    {
        PORTx->PCR[pin] = 0;

        PORTx->PCR[pin] |= PORT_PCR_MUX(pPinConfig->Mux);
        PORTx->PCR[pin] |= pPinConfig->Pull << 0;
        if (pPinConfig->Interrupt != PORT_INTERRUPT_DISABLE)
        {
            PORTx->PCR[pin] |= PORT_PCR_IRQC(pPinConfig->Interrupt);
            if (PORTx == PORTA || PORTx == PORTE)
            {
            	__NVIC_EnableIRQ(PORTAE_IRQn);
            }

            if (PORTx == PORTB || PORTx == PORTC || PORTx == PORTD)
            {
            	__NVIC_EnableIRQ(PORTBCD_IRQn);
            }
            s_PORT_Handler = pPinConfig->CallBack;
        }
    }

    return retVal;
}
void PORT_SetPriority(PORT_Type *port,uint8_t priority){
	if(port == PORTA || port == PORTE){
		__NVIC_SetPriority(PORTAE_IRQn, priority);
	}else{
		__NVIC_SetPriority(PORTBCD_IRQn, priority);
	}
}
static void ClearPortInterruptFlags(PORT_Type *port) {
	uint32_t interruptMask = port->ISFR;
    for (uint8_t i = 0; i < 32; i++) {
    	if (interruptMask & (1UL << i)) {
    		s_PORT_Handler(port,i);
            port->PCR[i] |= PORT_PCR_ISF(1); // clear ISFR flag
        }
    }
}
static void handlePortInterrupt(PORT_Type *port, uint8_t portIndex) {
    if (PCC->CLKCFG[portIndex] & PCC_CLKCFG_CGC_MASK) {
        if (port->ISFR != 0) {
            ClearPortInterruptFlags(port); // Xóa cờ ngắt
        }
    }
}

void PORTBCD_IRQHandler(void){
	if(NULL != s_PORT_Handler){
		 handlePortInterrupt(PORTB, PCC_PORTB_INDEX);
		 handlePortInterrupt(PORTC, PCC_PORTC_INDEX);
		 handlePortInterrupt(PORTD, PCC_PORTD_INDEX);
	}
}
void PORTAE_IRQHandler(void){

	if(NULL != s_PORT_Handler){
		handlePortInterrupt(PORTA, PCC_PORTA_INDEX);
		handlePortInterrupt(PORTE, PCC_PORTE_INDEX);
	}
}
