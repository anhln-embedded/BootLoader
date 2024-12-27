/*
 * led_service.c
 *
 *  Created on: Dec 22, 2024
 *      Author: nvd
 */

#include "led_service.h"
#define NUM_MODE 4
#define MAX_COUNT UINT32_MAX

volatile uint8_t g_mutexFlag = 0; // 0: unlocked, 1: locked
volatile uint32_t g_systickCounter;
volatile uint8_t mode = LED_MODE_1;
/*==========*/
static void mode0(void);
static void mode1(void);
static void mode2(void);
static void mode3(void);
static void turnOffAll(void);
static void initGPIO(void);
static void enableClock(void);
static void toggleMode();



LED_Mode LED_getMode(){
	return mode;
}
void LED_init(LED_Mode modeDefault){
	enableClock();
	initGPIO();
	turnOffAll();
	mode0();
	// init sys tick
	if (SysTick_Config(SystemCoreClock / 1000U))
	{
		while (1) {}  // Error handling, stay here if SysTick fails
	}
	mode = modeDefault;
}

void LED_loop()
{
    switch (mode)
    {
    case LED_MODE_1: // Mode 2
        mode1();
        break;
    case LED_MODE_2: // Mode 1
         mode2();
         break;
    case LED_MODE_3: // Mode 2
         mode3();
         break;
    default:
        break;
    }
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

static void initGPIO() {
    // Pin Configuration for LEDs
	PortConfig_t ledPinConfig = {PORT_MUX_GPIO, PORT_PULL_DISABLE, PORT_INTERRUPT_DISABLE, NULL};
	GPIOConfig_t ledConfig = {&ledPinConfig,OUTPUT, GPIO_HIGH};

    // Green LED configuration
    GPIO_Init(LED_PORT,gLED_PIN,&ledConfig);

    // Red LED configuration (using same PinConfig and GPIOConfig as Green LED)
    GPIO_Init(LED_PORT,rLED_PIN,&ledConfig);

    // Blue LED configuration (using same PinConfig and GPIOConfig as Green LED)
    GPIO_Init(bLED_PORT, bLED_PIN, &ledConfig);

    // Button (SW) configuration
    PortConfig_t buttonPinConfig = {PORT_MUX_GPIO, PORT_PULL_UP, PORT_INTERRUPT_FALLING, buttonCallback};
    GPIOConfig_t buttonConfig = {&buttonPinConfig,INPUT,GPIO_HIGH};
    GPIO_Init(SW_PORT,SW_PIN,&buttonConfig);
}

static void toggleMode(){
	mode = (mode+1)%NUM_MODE;
	turnOffAll();
}
static void mode0(){

	 static uint32_t previousMillis = 0;
	    static uint8_t ledState = 0;
	    uint32_t currentMillis = millis();

	    if ((currentMillis - previousMillis) >= 100)
	    {
	        previousMillis = currentMillis;
	        ledState = !ledState;           // toggle

	        if (ledState)
	        {
	        	GPIO_WritePin(bLED_GPIO, bLED_PIN, GPIO_LOW);  // Green LED ON
	        }
	        else
	        {
	        	GPIO_WritePin(bLED_GPIO, bLED_PIN, GPIO_HIGH);  // Green LED ON
	        }
	    }

}
static void mode1()
{
//    GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_LOW);  // Green LED ON
//    GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_LOW);  // Red LED ON

    static uint32_t previousMillis = 0;
    static uint8_t ledState = 0;
    uint32_t currentMillis = millis();

    if ((currentMillis - previousMillis) >= 1000)
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

static void mode2(){
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
static void mode3(void){
	turnOffAll();
}

static void turnOffAll(){
	  GPIO_WritePin(LED_GPIO, gLED_PIN, GPIO_HIGH);  // Green LED OFF
	  GPIO_WritePin(LED_GPIO, rLED_PIN, GPIO_HIGH);   // Red LED OFF
	  GPIO_WritePin(bLED_GPIO, bLED_PIN, GPIO_HIGH);   // Blue LED OFF

}


static void enableClock(void){
	PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1); /*enable port B for Button*/
	PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC(1); /*enable port D for RGB LED*/
}

