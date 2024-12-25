/*
 * PORT.c
 *
 *  Created on: Nov 30, 2024
 *      Author: nvd
 */

#include <stddef.h>
#include "my_gpio.h"
#include "my_status.h"
static GPIO_Type* portToGPIO(PORT_Type* portX) {
    if (portX == PORTA) {
        return GPIOA;
    } else if (portX == PORTB) {
        return GPIOB;
    } else if (portX == PORTC) {
        return GPIOC;
    } else if (portX == PORTD) {
        return GPIOD;
    } else if (portX == PORTE) {
        return GPIOE;
    }else {
        return NULL;
    }
}


status_t GPIO_Init(PORT_Type * portX, uint8_t pin, const GPIOConfig_t *gpioCf){
	if (!gpioCf) {
	        return STATUS_ERROR;
	   }
	GPIO_Type * gpioX = portToGPIO(portX);
	if(gpioX != NULL){
		// Initialize the port pin using the PORT driver
		if (PORT_Pin_Init(portX,pin, gpioCf->portCf) == STATUS_OK) {
		        // Set GPIO direction
		        if (gpioCf->direction == INPUT) {
		        	gpioX->PDDR &= ~GPIO_PDDR_PDD(1UL << pin); // Set as input
		        } else {
		            GPIO_WritePin(gpioX, pin, gpioCf->initialState);
		            gpioX->PDDR |= GPIO_PDDR_PDD(1UL << pin); // Set as output
		        }
		    }
	}else{
		return STATUS_ERROR;
	}

	return STATUS_OK;

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

