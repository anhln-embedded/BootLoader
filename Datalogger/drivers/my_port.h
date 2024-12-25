/*
 * port.h
 *
 *  Created on: Nov 24, 2024
 *      Author: Luu Ngoc Anh
 */

#ifndef MY_PORT_H_
#define MY_PORT_H_

#include "MKE16Z4.h"
#include "my_status.h"
/*******************************************************************************
* Definitions
******************************************************************************/


typedef enum {
    PORT_MUX_ANALOG = 0,
    PORT_MUX_GPIO = 1,
    PORT_MUX_ALT2 = 2,
    PORT_MUX_ALT3 = 3,
    PORT_MUX_ALT4 = 4,
    PORT_MUX_ALT5 = 5,
    PORT_MUX_ALT6 = 6,
    PORT_MUX_ALT7 = 7
} PortMuxType;

typedef enum {
    PORT_PULL_DISABLE = 0,
    PORT_PULL_DOWN = 2,
    PORT_PULL_UP = 3,
} PortPullType;


typedef enum {
    PORT_INTERRUPT_DISABLE = 0,
    PORT_INTERRUPT_LOGIC_0 = 8,
    PORT_INTERRUPT_RISING  = 9,
    PORT_INTERRUPT_FALLING = 10,
    PORT_INTERRUPT_EITHER  = 11,
    PORT_INTERRUPT_LOGIC_1 = 12,
} PortInterruptType;

typedef void (*CallBackType)(PORT_Type *port,uint8_t pin);

typedef struct {
    PortMuxType Mux;
    PortPullType Pull;
    PortInterruptType Interrupt;
    CallBackType CallBack;
} PortConfig_t;

/*******************************************************************************
* API
******************************************************************************/

status_t PORT_Pin_Init(
	PORT_Type * portInstance,
    uint8_t pin,
    const PortConfig_t *pPinConfig);
void PORT_SetPriority(PORT_Type *port,uint8_t priority);
#endif /* MY_PORT_H_ */
