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
<<<<<<< HEAD
    while (1)
    {
    	BootRun();
=======
//	FLASH_Erase(APPLICATION_ADRESS_1);
    while (1)
    {
    	BootFirmwareUpdate();
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
    }
}
