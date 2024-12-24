/*
 * msg_service.c
 *
 *  Created on: Dec 22, 2024
 *      Author: nvd
 */

#include "timer.h"
#include <stddef.h>
volatile uint8_t g_timerExpiredFlag = 0;

Timer_CallBackType g_callback = NULL;
void TIMER_init(uint32_t periousTime){
	//setup timer
	LPIT_init();
	LPIT_setCallBack(lpitCallback);
	LPIT_delay(periousTime);
	LPIT_startTimer(0);
}
void TIMER_setCallback(Timer_CallBackType callback){
	g_callback = callback;
}
void lpitCallback(){
	g_timerExpiredFlag =1;
	if(NULL != g_callback){
		g_callback();
		g_timerExpiredFlag = 0;
	}
}

uint8_t TIMER_timeout(){
	return g_timerExpiredFlag;
}
void TIMER_clear(){
	g_timerExpiredFlag =0;
}

