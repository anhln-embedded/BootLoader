/*
 * my_lpit.h
 *
 *  Created on: Dec 9, 2024
 *      Author: nvd
 */

#ifndef MY_LPIT_H_
#define MY_LPIT_H_
#include "MKE16Z4.h"
#include "stdbool.h"

typedef void (*LPIT_CallBackType)(void); // Function pointer type
void LPIT_enableClock();
void LPIT_init();
void LPIT_startTimer(uint8_t channel);
void LPIT_delay(uint32_t time);
void LPIT_setCallBack(LPIT_CallBackType function);
void LPIT_SetPriority(uint8_t priority);
#endif /* MY_LPIT_H_ */
