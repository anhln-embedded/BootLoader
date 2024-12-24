#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "led_service.h"
#include "sensor_service.h"
#include "timer.h"
#include "my_lpuart.h"

#define MAX_JSON_SIZE 100
#define LPUART_BASE LPUART2
#define BAUDRATE 4800


int8_t values[2] = {0};
char* msg;
void sendMsg(){
	 snprintf(msg, MAX_JSON_SIZE, "%d,%d\n", values[0], values[1]);
    LPUART_println(msg);
}
int main(void)
{
	msg = (char*)malloc(MAX_JSON_SIZE);

	LPUART_begin(LPUART_BASE,BAUDRATE);
	LPUART_println("MSG starting ..");

	TEMP_init();
	LED_init(LED_MODE_1);
	TIMER_init(10000);
	TIMER_setCallback(sendMsg);


    while (1) {
    	LED_start();
    	values[0] = (int8_t)TEMP_getAvrValue(); // temperature
        values[1] = LED_getMode(); // mode

    }
    free(msg);
    return 0;
}


