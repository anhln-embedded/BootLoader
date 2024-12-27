/*
 * KE16_Flash.c
 *
 *  Created on: Dec 19, 2024
 *      Author: ADMIN
 */

#include "KE16_Flash.h"

#define FTFA_FSTAT_CLEAR_ERROR          0x30U
#define START_COMMAND                   0x80U
#define CMD_PROGRAM_LONGWORD            0x06U
#define CMD_ERASE_SECTOR               	0x09U

void FLASH_Write(uint32_t Address, uint32_t Data)
{
    /* Wait Previous Command Complete? */
<<<<<<< HEAD
	__disable_irq();
=======
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);

    /* Check Error from Previous Command */
    if ( ((FTFA->FSTAT & FTFA_FSTAT_ACCERR_MASK) == FTFA_FSTAT_ACCERR_MASK) \
        || ((FTFA->FSTAT & FTFA_FSTAT_FPVIOL_MASK) == FTFA_FSTAT_FPVIOL_MASK) )
    {
        FTFA->FSTAT = FTFA_FSTAT_CLEAR_ERROR;
    }

    /*write command and parameter to FCCOB */
    FTFA->FCCOB0 = CMD_PROGRAM_LONGWORD;
    /*set address*/
    FTFA->FCCOB1 = (uint8_t) (Address >>16);
    FTFA->FCCOB2 = (uint8_t) (Address >>8);
    FTFA->FCCOB3 = (uint8_t) (Address);

    /*set data*/
    FTFA->FCCOB4 = (uint8_t) (Data >> 24);
    FTFA->FCCOB5 = (uint8_t) (Data >> 16);
    FTFA->FCCOB6 = (uint8_t) (Data >> 8);
    FTFA->FCCOB7 = (uint8_t) (Data);

    /*clear CCIF Flag to launch command */
<<<<<<< HEAD
    FTFA->FSTAT = START_COMMAND;
    while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
    __enable_irq();
=======
//    FTFA->FSTAT = START_COMMAND;
	/* Start command and wait for it to complete */
	__disable_irq();
	FTFA->FSTAT |= FTFA_FSTAT_CCIF(1U);
	while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
	__enable_irq();
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
}

void FLASH_Erase(uint32_t Address)
{
	if(Address < 0x800UL)
	{
		return;
	}

   /* wait previous command complate*/
<<<<<<< HEAD
  __disable_irq();
=======
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
  while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);

  /*write command and parameter to FCCOB0*/
  FTFA->FCCOB0 = CMD_ERASE_SECTOR ;
  /*set address*/
  FTFA->FCCOB1 = (uint8_t) (Address >>16);
  FTFA->FCCOB2 = (uint8_t) (Address >>8);
  FTFA->FCCOB3 = (uint8_t) (Address);

  //clear Clear CCIF;
<<<<<<< HEAD
  FTFA->FSTAT = START_COMMAND;
  while ((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
  __enable_irq();
=======
  //FTFA->FSTAT = START_COMMAND;
	/* Start command and wait for it to complete */
	__disable_irq();
	FTFA->FSTAT |= FTFA_FSTAT_CCIF(1U);
	while((FTFA->FSTAT & FTFA_FSTAT_CCIF_MASK) == 0);
	__enable_irq();

>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
}

void Flash_EraseSector(uint32_t SectorNum)
{
	if(0 == SectorNum || 1 == SectorNum)
	{
		return;
	}

	uint32_t Address = SectorNum * 1024;
<<<<<<< HEAD
	FLASH_Erase(Address);
=======
	Flash_Erase(Address);
>>>>>>> af3e224ceb77975899ef88e1c09f43064e1b440a
}

uint32_t Flash_Read(uint32_t Address)
{
	return *(uint32_t*)Address;
}
