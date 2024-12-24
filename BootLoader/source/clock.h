/*
 * clock.h
 *
 *  Created on: Nov 26, 2024
 *      Author: hnaco
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "MKE16Z4.h"

void EnableClockForPort(uint8_t PCC_PORT_X_INDEX);
void DisableClockForPort(uint8_t PCC_PORT_X_INDEX);

#endif /* CLOCK_H_ */
