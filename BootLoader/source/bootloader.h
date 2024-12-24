/*
 * bootloader.h
 *
 *  Created on: Dec 21, 2024
 *      Author: hnaco
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include <stdarg.h>
#include <stdio.h>

#include "MKE16Z4.h"
#include "bootloader.h"
#include "KE16_Flash.h"
#include "parsing_srec.h"
#include "uart.h"
#include "clock.h"
#include "port.h"
#include "circular_queue.h"


#define APPLICATION_ADRESS_1 0x5000

void BootInit(void);
void BootJumpToApplication(uint32_t address);
void BootFirmwareUpdate(void);

#endif /* BOOTLOADER_H_ */
