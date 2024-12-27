/*
 * clock.c
 *
 *  Created on: Nov 26, 2024
 *      Author: hnaco
 */

#include "clock.h"

void EnableClockForPort(uint8_t PCC_PORT_X_INDEX)
{
    PCC->CLKCFG[PCC_PORT_X_INDEX] |= PCC_CLKCFG_CGC(1);
}

void DisableClockForPort(uint8_t PCC_PORT_X_INDEX)
{
    PCC->CLKCFG[PCC_PORT_X_INDEX] &= ~PCC_CLKCFG_CGC(1);
}