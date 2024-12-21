/*
 * bootloader.h
 *
 *  Created on: Dec 21, 2024
 *      Author: hnaco
 */

#ifndef BOOTLOADER_H_
#define BOOTLOADER_H_

#include "MKE16Z4.h"

#define APPLICATION_ADRESS_1 0x3000

void BootInit(void);
void BootJumpToApplication(uint32_t address);
void BootFirmwareUpdate(void);

#endif /* BOOTLOADER_H_ */
