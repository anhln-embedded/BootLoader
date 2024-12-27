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
#include "share_memory.h"


#define APPLICATION_ADDRESS_1 0x5000

typedef enum {
    BOOT_JUMPTO_APP_SUCCESS,
    BOOT_JUMPTO_APP_FAIL,
    UPDATE_SUCCESS,
    UPDATE_FAIL,
}Boot_status_t;

void BootInit(void);
void BootRun(void);
void BootJumpToApplication(uint32_t address);
Boot_status_t BootFirmwareUpdate(uint32_t address);

#endif /* BOOTLOADER_H_ */
