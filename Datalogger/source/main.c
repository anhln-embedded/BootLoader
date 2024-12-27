#include <stdint.h>
<<<<<<< HEAD
#include <stdlib.h>
=======
#include <stdio.h>
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
#include <string.h>
#include "led_service.h"
#include "sensor_service.h"
#include "timer.h"
#include "serial.h"
<<<<<<< HEAD
#include "share_memory.h"

#define MAX_JSON_SIZE 50
#define LPUART_BASE LPUART2
#define BAUDRATE 4800

__attribute__((section(".share_memory"))) uint32_t BOOT_MODE;


=======

#define MAX_JSON_SIZE 100
#define LPUART_BASE LPUART0
#define BAUDRATE 4800

>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
char * data;
int8_t values[2] = {0};
char* msg;
void sendMsg(){
	 snprintf(msg, MAX_JSON_SIZE, "%d,%d\n", values[0], values[1]);
<<<<<<< HEAD
	 Serial_print(msg);
=======
	 Serial_println(LPUART0,msg);
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
}
int main(void)
{
	msg = (char*)malloc(MAX_JSON_SIZE);

<<<<<<< HEAD
	Serial_begin(LPUART_BASE, BAUDRATE);
	//Serial_println(LPUART_BASE,"MSG starting ..");
	Serial_print("KE16 Starting ...");

	TEMP_init();
	LED_init(LED_MODE_1);
	TIMER_init(10000);
=======
	Serial_begin(LPUART0, 4800);
	Serial_println(LPUART0,"MSG starting ..");

	TEMP_init();
	LED_init(LED_MODE_1);
	TIMER_init(5000);
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
	TIMER_setCallback(sendMsg);


    while (1) {
    	LED_loop();
    	values[0] = (int8_t)TEMP_getAvrValue(); // temperature
        values[1] = LED_getMode(); // mode

<<<<<<< HEAD
        if(Serial_available(4)){
        	 data = Serial_readBytes();
        	 if(strcmp(data, "ffff") == 0){
        		 Serial_print("Reset Chip !");
        		 BOOT_MODE = 0xAAAAAAAA;
        		 NVIC_SystemReset();
        	 }else{
        		 Serial_clear();
        		 Serial_print("ERROR\n");
=======
        if(Serial_available(LPUART0,4)){
        	 data = Serial_readBytes(LPUART0);
        	 if(strcmp(data, "ffff") == 0){
        		 Serial_println(LPUART0,"Reset Chip !");
        		 NVIC_SystemReset();
        	 }else{
            	 Serial_println(LPUART0,"ERROR");
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
        	 }
        }

    }
    free(msg);
    return 0;
}


