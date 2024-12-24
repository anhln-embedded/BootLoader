#ifndef MY_CLOCK_H_
#define MY_CLOCK_H_

#include "MKE16Z4.h"

void CLOCK_enable(uint8_t index);
uint32_t CLOCK_getFreq(void);
#endif /* MY_CLOCK_H_ */
