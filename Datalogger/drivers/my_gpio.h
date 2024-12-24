#ifndef MY_GPIO_H_
#define MY_GPIO_H_

#include "MKE16Z4.h"
typedef enum {
    ISF_DISABLED =0,          // 0: ISF flag is disabled

    ISF_RESERVED_1 = 4 ,        // 1: Reserved
    ISF_RESERVED_2 ,        // 2: Reserved
    ISF_RESERVED_3 ,        // 3: Reserved
	ISF_RESERVED_4 ,
    ISF_LOGIC_0 ,           // 4: ISF flag and Interrupt when logic 0
    ISF_RISING_EDGE,       // 5: ISF flag and Interrupt on rising-edge
    ISF_FALLING_EDGE,      // 6: ISF flag and Interrupt on falling-edge
    ISF_EITHER_EDGE,       // 7: ISF flag and Interrupt on either edge
    ISF_LOGIC_1,           // 8: ISF flag and Interrupt when logic 1
    ISF_RESERVED_5,        // 9: Reserved
    ISF_RESERVED_6,        // 10: Reserved
    ISF_RESERVED_7         // 11: Reserved
} ISFFlag_t;

typedef enum {
    GPIO_LOW,
    GPIO_HIGH
} PinState_t;

typedef enum {
    MUX_ANALOG,        // Pin disabled (Analog mode)
    MUX_GPIO,          // GPIO mode (General Purpose Input/Output)
    MUX_ALT2,          // Alternative 2
    MUX_ALT3,          // Alternative 3
    MUX_ALT4,          // Alternative 4
    MUX_ALT5,          // Alternative 5
    MUX_ALT6,          // Alternative 6
    MUX_ALT7           // Alternative 7
} PinMux_t;

typedef enum{
	PULL_DOWN,
	PULL_UP,
	PULL_DISABLE
}Pull_t;

typedef enum{
	INPUT,
	OUTPUT
}PinMode_t;

// Callback function type for interrupts
typedef void (*CallBackType)(PORT_Type *port,uint8_t pin);

// PORT configuration
typedef struct {
    PORT_Type *port;
    uint8_t pin;
    Pull_t pull;          /* None, Pull-up, Pull-down*/
    ISFFlag_t irqConfig;     /* Interrupt mode (Rising, Falling, etc.)*/
} PortConfig_t;

// GPIO configuration
typedef struct {
    GPIO_Type *gpio;
    PinMode_t direction;     /* INPUT (0) or OUTPUT (1)*/
    PinState_t initialState; /* Initial state*/
    CallBackType callback; /* Interrupt callback function*/
} GPIOConfig_t;

/* API*/

void GPIO_Init(const PortConfig_t *portConfig, const GPIOConfig_t *pinConfig);
void GPIO_WritePin(GPIO_Type *gpio, uint8_t pin, PinState_t state);
PinState_t GPIO_ReadPin(GPIO_Type *gpio, uint8_t pin);
void GPIO_TogglePin(GPIO_Type *gpio, uint8_t pin);
void handlePortInterrupt(PORT_Type *port, uint8_t portIndex);
void GPIO_SetPriority(PORT_Type *port,uint8_t priority);
#endif /* MY_GPIO_H_ */
