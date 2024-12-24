/*
 * led_service.h
 *
 *  Created on: Dec 22, 2024
 *      Author: nvd
 */
#include "my_lpit.h"
#include "my_gpio.h"
#include <stdint.h>
#include <stddef.h>

#ifndef LED_SERVICE_H_
#define LED_SERVICE_H_


#define LED_PORT PORTB
#define LED_GPIO GPIOB
#define gLED_PIN 4
#define rLED_PIN 5

#define bLED_PORT PORTD
#define bLED_PIN 1


#define SW_PORT PORTD
#define SW_GPIO GPIOD
#define SW_PIN 2  //sw3


typedef enum {
	LED_MODE_1,
	LED_MODE_2,
	LED_MODE_3
}LED_Mode;

LED_Mode LED_getMode();
void LED_init(LED_Mode mode);
void LED_start();
void toggleMode();
void mode1(void);
void mode2(void);
void mode3(void);
void turnOffAll(void);
void initGPIO(void);
void enableClock(void);
void SysTick_DelayTicks(uint32_t n);
uint32_t millis(void);

void buttonCallback(PORT_Type *port, uint8_t pin);
void lpitCallback(void);

#endif /* LED_SERVICE_H_ */
