#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "led_service.h"
#include "sensor_service.h"
#include "timer.h"
#include "serial.h"
#include "share_memory.h"

#define MAX_JSON_SIZE 100
#define LPUART_BASE LPUART0
#define BAUDRATE 4800

__attribute__((section(".share_memory"))) uint32_t BOOT_MODE;


char * data;
int8_t values[2] = {0};
char* msg;
void sendMsg(){
	 snprintf(msg, MAX_JSON_SIZE, "%d,%d\n", values[0], values[1]);
	 Serial_println(LPUART0,msg);
}
int main(void)
{
	msg = (char*)malloc(MAX_JSON_SIZE);

	Serial_begin(LPUART0, 9600);
	Serial_println(LPUART0,"MSG starting ..");

	TEMP_init();
	LED_init(LED_MODE_1);
	TIMER_init(5000);
	TIMER_setCallback(sendMsg);


    while (1) {
    	LED_loop();
    	values[0] = (int8_t)TEMP_getAvrValue(); // temperature
        values[1] = LED_getMode(); // mode

        if(Serial_available(LPUART0,4)){
        	 data = Serial_readBytes(LPUART0);
        	 if(strcmp(data, "ffff") == 0){
        		 Serial_println(LPUART0,"Reset Chip !");
        		 BOOT_MODE = 0xAAAAAAAA;
        		 NVIC_SystemReset();
        	 }else{
            	 Serial_println(LPUART0,"ERROR");
        	 }
        }

    }
    free(msg);
    return 0;
}


