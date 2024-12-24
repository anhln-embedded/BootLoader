/*
 * main.c
 *
 *  Created on: Dec 19, 2024
 *      Author: hnaco
 */

#include "MKE16Z4.h"
#include "bootloader.h"


int main()
{
	BootInit();
//	FLASH_Erase(APPLICATION_ADRESS_1);
    while (1)
    {
    	BootFirmwareUpdate();
    }
}
