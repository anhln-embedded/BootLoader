/*
 * bootloader.c
 *
 *  Created on: Dec 21, 2024
 *      Author: hnaco
 */

#include "bootloader.h"

void BootInit(void){
	__enable_irq();
}

void BootJumpToApplication(uint32_t address){
    /* Disable interrupts before jump to Apptication */
    __disable_irq();
    /* Set the MSP to the application MSP */
    __set_MSP(*(volatile uint32_t *)address);
    SCB->VTOR = address;
    /* Set the PC to the application reset vector */
    void (*app_reset_handler)(void) = (void (*)(void))(*(uint32_t *)(address + 4));
    app_reset_handler();
}
void BootFirmwareUpdate(void){

}
