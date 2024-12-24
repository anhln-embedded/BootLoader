/*
 * PORT.c
 *
 *  Created on: Nov 30, 2024
 *      Author: nvd
 */

#include <stddef.h>
#include "my_gpio.h"

volatile CallBackType _callback = NULL;
//	LED_PORT->PCR[gLED_PIN] |= PORT_PCR_MUX(1);
//	LED_PORT->PCR[rLED_PIN] |= PORT_PCR_MUX(1);
//
//	/*set up pin mode*/
//	LED_GPIO->PDDR |=  GPIO_PDDR_PDD(1 << gLED_PIN); /*pin 4 -> output*/
//	LED_GPIO->PDDR |= GPIO_PDDR_PDD(1 << rLED_PIN); /*pin 5 -> output*/
//	LED_GPIO->PSOR |= GPIO_PSOR_PTSO((1<<rLED_PIN)|((1<<gLED_PIN))|(1<<bLED_PIN));

void GPIO_Init(const PortConfig_t *portCf, const GPIOConfig_t *gpioCf){
	PORT_Type *_port = portCf->port;
	uint8_t pin = portCf->pin;
	/*init port*/
	_port->PCR[pin] |= PORT_PCR_MUX(MUX_GPIO);
	/*set pull*/
	if(portCf->pull != PULL_DISABLE){
		_port->PCR[pin] |= PORT_PCR_PE(1);
		_port->PCR[pin] |= PORT_PCR_PS(portCf->pull);
	}

	/*set direction*/
	if(gpioCf->direction == INPUT){
		gpioCf->gpio->PDDR &= GPIO_PDDR_PDD(~(1UL << pin));
	}else{
		GPIO_WritePin(gpioCf->gpio, pin, gpioCf->initialState);
		gpioCf->gpio->PDDR |= GPIO_PDDR_PDD(OUTPUT << pin);
	}

	/*interupt -> port pin detect AE ,BCD*/
	/*NMI_b -> PTD3 cannot change NV_FOPT_NMI_DIS()*/
	if(portCf->irqConfig != ISF_DISABLED){
		_port->PCR[pin] |= PORT_PCR_IRQC(portCf->irqConfig);
		/*NVIC enable interupt IRQ number */
		/* AE = 6
		 * BCD = 26 */
		if(_port == PORTA || _port == PORTE){
			__NVIC_EnableIRQ(PORTAE_IRQn);

		}else{
			__NVIC_EnableIRQ(PORTBCD_IRQn);
		}
		_callback = gpioCf->callback;
	}

}
void GPIO_SetPriority(PORT_Type *port,uint8_t priority){
	if(port == PORTA || port == PORTE){
		__NVIC_SetPriority(PORTAE_IRQn, priority);
	}else{
		__NVIC_SetPriority(PORTBCD_IRQn, priority);
	}
}

void ClearPortInterruptFlags(PORT_Type *port) {
	uint32_t interruptMask = port->ISFR;
    for (uint8_t i = 0; i < 32; i++) {
    	if (interruptMask & (1UL << i)) {
        	_callback(port,i);
            port->PCR[i] |= PORT_PCR_ISF(1); // clear ISFR flag
        }
    }
}
void handlePortInterrupt(PORT_Type *port, uint8_t portIndex) {
    if (PCC->CLKCFG[portIndex] & PCC_CLKCFG_CGC_MASK) {
        if (port->ISFR != 0) {
            ClearPortInterruptFlags(port); // Xóa cờ ngắt
        }
    }
}

void PORTBCD_IRQHandler(void){
	if(NULL != _callback){
		 handlePortInterrupt(PORTB, PCC_PORTB_INDEX);
		 handlePortInterrupt(PORTC, PCC_PORTC_INDEX);
		 handlePortInterrupt(PORTD, PCC_PORTD_INDEX);
	}
	//clear pending when duplicate command in same time

}
void PORTAE_IRQHandler(void){

	if(NULL != _callback){
		handlePortInterrupt(PORTA, PCC_PORTA_INDEX);
		handlePortInterrupt(PORTE, PCC_PORTE_INDEX);
	}
}

void GPIO_WritePin(GPIO_Type *gpio, uint8_t pin, PinState_t state){
	if(state == GPIO_LOW){
		gpio->PCOR |= GPIO_PCOR_PTCO(1 << pin);
	}else{// HIGH
		gpio->PSOR |= GPIO_PSOR_PTSO(1 << pin);
	}
}
PinState_t GPIO_ReadPin(GPIO_Type *gpio, uint8_t pin){
	return (gpio->PDIR >> pin) & 0x01UL;
}
void GPIO_TogglePin(GPIO_Type *gpio, uint8_t pin){
	gpio->PTOR |= GPIO_PTOR_PTTO(1<<pin);
}

