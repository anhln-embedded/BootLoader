/*
 * port.h
 *
 *  Created on: Nov 24, 2024
 *      Author: Luu Ngoc Anh
 */

#ifndef PORT_H_
#define PORT_H_

#include "MKE16Z4.h"

/*******************************************************************************
* Definitions
******************************************************************************/

typedef enum {
    STATUS_ERROR = 0,
    STATUS_OK = 1,
    STATUS_BUSY,
    STATUS_TIMEOUT
} status_t;

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
    PORT_INS_A,
    PORT_INS_B,
    PORT_INS_C,
    PORT_INS_D,
    PORT_INS_E,
    PORT_INS_NUM,
} Port_Instance_t;

static PORT_Type *s_PortInstance[] = PORT_BASE_PTRS;

typedef enum {
    PORT_INTERRUPT_DISABLE = 0,
    PORT_INTERRUPT_LOGIC_0 = 8,
    PORT_INTERRUPT_RISING  = 9,
    PORT_INTERRUPT_FALLING = 10,
    PORT_INTERRUPT_EITHER  = 11,
    PORT_INTERRUPT_LOGIC_1 = 12,
} PortInterruptType;

typedef void (*CallBackType)(uint8_t);

typedef struct {
    PortMuxType Mux;
    PortPullType Pull;
    PortInterruptType Interrupt;
    CallBackType CallBack;
} PinConfigType;

/*******************************************************************************
* API
******************************************************************************/

status_t PORT_Pin_Init(
    Port_Instance_t portInstance,
    uint8_t pin,
    const PinConfigType *pPinConfig);

#endif /* PORT_H_ */