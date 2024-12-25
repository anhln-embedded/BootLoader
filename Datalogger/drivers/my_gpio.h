#ifndef MY_GPIO_H_
#define MY_GPIO_H_

#include "MKE16Z4.h"
#include "my_port.h"

/*******************************************************************************
* API
******************************************************************************/

typedef enum {
    GPIO_LOW,
    GPIO_HIGH
} PinState_t;

typedef enum {
    INPUT,
    OUTPUT
} PinMode_t;

typedef struct {
         /* GPIO instance */
	const PortConfig_t * portCf;
    PinMode_t direction;     /* INPUT (0) or OUTPUT (1) */
    PinState_t initialState; /* Initial state */
} GPIOConfig_t;

/* API */
status_t GPIO_Init(PORT_Type * portX, uint8_t pin, const GPIOConfig_t *gpioCf);

void GPIO_WritePin(GPIO_Type *gpio, uint8_t pin, PinState_t state);

PinState_t GPIO_ReadPin(GPIO_Type *gpio, uint8_t pin);

void GPIO_TogglePin(GPIO_Type *gpio, uint8_t pin);
#endif /* MY_GPIO_H_ */
