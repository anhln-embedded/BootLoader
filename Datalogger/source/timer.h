/*
 * communication_service.h
 *
 *  Created on: Dec 22, 2024
 *      Author: nvd
 */

#ifndef MSG_SERVICE_H_
#define MSG_SERVICE_H_
#include "my_lpit.h"

typedef void (*Timer_CallBackType)(void); // Function pointer type
void TIMER_init(uint32_t periousTime);
void lpitCallback();
uint8_t TIMER_timeout();
void TIMER_clear();
void TIMER_setCallback(Timer_CallBackType callback);
#endif /* MSG_SERVICE_H_ */
