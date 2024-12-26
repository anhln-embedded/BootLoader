/*
 * main.c
 *
 *  Created on: Dec 19, 2024
 *      Author: hnaco
 */

#include "MKE16Z4.h"
#include "bootloader.h"

void delay(){
	for(int i = 0; i < 10000; i++){

	}
}



int main()
{
	BootInit();
	delay();
//	BootJumpToApplication(0x5000);
    while (1)
    {
    	BootFirmwareUpdate();
    }
}
