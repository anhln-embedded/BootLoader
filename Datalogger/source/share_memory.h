/*
 * share_memory.h
 *
 *  Created on: Dec 27, 2024
 *      Author: hnaco
 */

#ifndef SHARE_MEMORY_H_
#define SHARE_MEMORY_H_

typedef enum {
    BOOR_MODE_RUN_APP,
    BOOT_MODE_UPDATE_FIRM_WARE,
}BOOT_MODE_t;

extern __attribute__((section(".share_memory"))) BOOT_MODE_t BOOT_MODE;

#endif /* SHARE_MEMORY_H_ */
