/*
 * share_memory.h
 *
 *  Created on: Dec 27, 2024
 *      Author: hnaco
 */

#ifndef SHARE_MEMORY_H_
#define SHARE_MEMORY_H_

typedef enum {

}BOOT_MODE_t

extern __attribute__((section(".share_memory"))) uint8_t BOOT_MODE;

#endif /* SHARE_MEMORY_H_ */
