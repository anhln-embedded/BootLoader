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
    while (1)
    {
    	BootRun();
    }
}
