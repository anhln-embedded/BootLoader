/*
 * led_service.c
 *
 *  Created on: Dec 22, 2024
 *      Author: nvd
 */

#include "led_service.h"
#define NUM_MODE 2
#define MAX_COUNT UINT32_MAX

volatile uint8_t g_mutexFlag = 0; // 0: unlocked, 1: locked
volatile uint32_t g_systickCounter;
volatile uint8_t mode = LED_MODE_1;
void toggleMode(){
	mode = (mode+1)%NUM_MODE;
	turnOffAll();
}
LED_Mode LED_getMode(){
	return mode;
}
void LED_init(LED_Mode modeDefault){
	enableClock();
	initGPIO();
	// init sys tick
	if (SysTick_Config(SystemCoreClock / 1000U))
	{
		while (1) {}  // Error handling, stay here if SysTick fails
	}
	mode = modeDefault;
}
void LED_start()
{
    switch (mode)
    {
    case LED_MODE_1: // Mode 1
        mode1();
        break;
    case LED_MODE_2: // Mode 2
        mode2();
        break;
    default:
        break;
    }
}


void mode1()
{
//    GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_LOW);  // Green LED ON
//    GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_LOW);  // Red LED ON

    static uint32_t previousMillis = 0;
    static uint8_t ledState = 0;
    uint32_t currentMillis = millis();

    if ((currentMillis - previousMillis) >= 200)
    {
        previousMillis = currentMillis;
        ledState = !ledState;           // toggle

        if (ledState)
        {
            GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_LOW);  // Green LED ON
            GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_LOW);  // Red LED ON
        }
        else
        {
            GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_HIGH); // Green LED OFF
            GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_HIGH); // Red LED OFF
        }
    }
}

void mode2(){
//    GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_LOW); // Green LED ON
//    GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_HIGH);  // Red LED OFF

    static uint32_t previousMillis = 0;
    static uint8_t ledState = 0;
    uint32_t currentMillis = millis();

    if ((currentMillis - previousMillis) >= 200)
    {
        previousMillis = currentMillis;
        ledState = !ledState;           // toggle

        if (ledState)
        {
            GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_LOW); // Green LED ON
            GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_HIGH);  // Red LED OFF
        }
        else
        {
        	GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_HIGH);  // Green LED OFF
        	GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_LOW); // Red LED ON
        }
    }

}
void mode3(void){
	turnOffAll();
}

void turnOffAll(){
	  GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_HIGH);  // Green LED OFF
	  GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_HIGH);   // Red LED OFF
	  GPIO_WritePin(bLED_PORT, bLED_PIN, GPIO_HIGH);   // Blue LED OFF

}
void initGPIO(){
	/* LED configuration (Green, Red, Blue)*/
	PortConfig_t ledPortConfig = {LED_PORT, gLED_PIN, PULL_DISABLE, ISF_DISABLED};
	GPIOConfig_t ledConfig = {LED_GPIO, OUTPUT, GPIO_HIGH, NULL};

	PortConfig_t redLedPortConfig = {LED_PORT, rLED_PIN, PULL_DISABLE, ISF_DISABLED};
	GPIOConfig_t redLedConfig = {LED_GPIO, OUTPUT, GPIO_HIGH, NULL};

	PortConfig_t blueLedPortConfig = {bLED_PORT, bLED_PIN, PULL_DISABLE, ISF_DISABLED};
	GPIOConfig_t blueLedConfig = {bLED_PORT, OUTPUT, GPIO_HIGH, NULL};

	/* Initialize GPIO for LEDs*/
	GPIO_Init(&ledPortConfig, &ledConfig);
	GPIO_Init(&redLedPortConfig, &redLedConfig);
	GPIO_Init(&blueLedPortConfig, &blueLedConfig);

	/* Button (SW) configuration*/
	PortConfig_t swPortConfig = {SW_PORT, SW_PIN, PULL_UP, ISF_FALLING_EDGE};
	GPIOConfig_t swConfig = {SW_GPIO, INPUT, GPIO_HIGH, buttonCallback};

	/* Initialize GPIO for button*/
	GPIO_Init(&swPortConfig, &swConfig);

}
void enableClock(void){
	PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1); /*enable port B for Button*/
	PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC(1); /*enable port D for RGB LED*/
}

void buttonCallback(PORT_Type *port, uint8_t pin){
	if(port == SW_PORT && pin == SW_PIN){
		toggleMode();
		}

}


/* SysTick Handler */
void SysTick_Handler(void)
{
	g_systickCounter++;
	if (g_systickCounter >= MAX_COUNT){
	        g_systickCounter = 0;
	 }
}

uint32_t millis(void)
{

    return g_systickCounter;
}
