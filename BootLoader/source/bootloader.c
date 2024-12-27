/*
 * bootloader.c
 *
 *  Created on: Dec 21, 2024
 *      Author: hnaco
 */

#include "bootloader.h"

__attribute__((section(".share_memory"))) uint32_t BOOT_MODE;


LPUART_InitStruct_t LPUART_InitStruct = {
    .BaudRate = 4800,
    .NumberData = 8,
    .ParityConfig = LPUART_PARITY_NONE,
    .MSB_First = 0,
    .PolarityConfig = {
        .LPUART_TxPolarity = 0,
        .LPUART_RxPolarity = 0},
    .StopBitConfig = LPUART_STOPBITNUMBER_1,
    .Frequency = 48000000,
    .EnableRxInterrupt = 1};

PinConfigType config = {
    .Mux = PORT_MUX_ALT2,
    .Interrupt = 0,
    .CallBack = 0,
};

SrecLine_t Data;
CircularQueue_t queue;
uint8_t line[100];
uint8_t lineIndex = 0;

__attribute__((section(".user_data"))) uint8_t isValid;


static uint8_t Flash_SrecLine(SrecLine_t *Data);
static void Erase_Application(uint32_t address);
static void UART_ISR(void);
static void Serial_Printf(char *format, ...);

void BootInit(void)
{
    __enable_irq();
    /*Debug*/
    /* Enable clock for PORTB */
    EnableClockForPort(PCC_PORTD_INDEX);
    PORT_Pin_Init(PORT_INS_D, 6, &config);
    PORT_Pin_Init(PORT_INS_D, 7, &config);
    UART_Init(LPUART2, &LPUART_InitStruct);
    UART_RegisterHandler(LPUART2, UART_ISR);
    Serial_Printf("address: 0x%08x\n", &BOOT_MODE);

    if(isValid == 0){
    	uint32_t data = *(uint32_t *)APPLICATION_ADDRESS_1;
    	if( data != 0xFFFFFFFF || data != 0){
    		Serial_Printf("Remove invalid program", &BOOT_MODE);
    		Erase_Application(APPLICATION_ADDRESS_1);
    	}

    }
}

void BootJumpToApplication(uint32_t address)
{
    /* Disable interrupts before jump to Apptication */
    __disable_irq();
    /* Set the MSP to the application MSP */
    __set_MSP(*(volatile uint32_t *)address);
    SCB->VTOR = address;
    /* Set the PC to the application reset vector */
    void (*app_reset_handler)(void) = (void (*)(void))(*(uint32_t *)(address + 4));
    app_reset_handler();
}

Boot_status_t BootFirmwareUpdate(uint32_t address)
{
    Boot_status_t status = UPDATE_FAIL;
    Erase_Application(address);

    while (1)
    {
        uint8_t charSrec;
        if (!CircularQueue_IsEmpty(&queue))
        {
            CircularQueue_Dequeue(&queue, &charSrec);
            line[lineIndex] = charSrec;
            if (charSrec == '\n')
            {
                lineIndex++;
                line[lineIndex] = 0;
                if (SrecReadLine((char *)line, &Data) == SREC_ERROR_NOERR)
                {
                    if (Data.u8SrecType == 0)
                    {
                        Serial_Printf("Boot Start\n");
                    }
                    else if (Data.u8SrecType == 9)
                    {
                        Serial_Printf("Boot End\n");
                        isValid = 1;
                        status = UPDATE_SUCCESS;
                        break;
                    }
                    else
                    {
                        Serial_Printf(".");
                        if (*(uint32_t *)(Data.u32Address) != 0xFFFFFFFF)
                        {
                            Serial_Printf("Can't flash\n");
                        }
                        Flash_SrecLine(&Data);
                    }
                }
                else
                {
                    status = UPDATE_FAIL;
                }



                // Flash_SrecLine(&Data);
                lineIndex = 0;
                line[lineIndex] = 0;
            }
            else
            {
                lineIndex++;
            }
        }
    }
    return status;
}

void BootRun(void)
{
    if (BOOT_MODE == 0xAAAAAAAA)
    {
        //Serial_Printf("\nDo you want to update to the new firmware? (y/n): ");
    	Serial_Printf("ke16hello");
        char c;
        while (CircularQueue_IsEmpty(&queue))
        {
            /*wait input*/
        }
        CircularQueue_Dequeue(&queue, &c);
        if (c == 'y')
        {
            Serial_Printf("Update Firmware\n");
            if (BootFirmwareUpdate(APPLICATION_ADDRESS_1) == UPDATE_SUCCESS)
            {
                Serial_Printf("Update Success\n");
                BOOT_MODE = 0x00;
            }
            else
            {
                Serial_Printf("Update Fail\n");
                BOOT_MODE = 0xAAAAAAAA;
            }
        }
        else
        {
            BOOT_MODE = 0x00;
        }
    }
    else
    {
        if (*(uint32_t *)APPLICATION_ADDRESS_1 != 0xFFFFFFFF)
        {
            Serial_Printf("Run Application\n");
            BootJumpToApplication(APPLICATION_ADDRESS_1);
        }
        else
        {
            Serial_Printf("No Application\n");
            BOOT_MODE = 0xAAAAAAAA;
        }
    }
}

static uint8_t Flash_SrecLine(SrecLine_t *Data)
{
    uint32_t dataFlash = 0;
    for (int i = 0; i < (Data->u8ByteCount - 4); i += 4)
    {
        dataFlash = (Data->pData[i + 3] << 24) | (Data->pData[i + 2] << 16) | (Data->pData[i + 1] << 8) | Data->pData[i];
        FLASH_Write(Data->u32Address + i, dataFlash);
    }
    return 1;
}

static void Erase_Application(uint32_t address)
{
    while ((0xffffffff != *(uint32_t *)address))
    {
        FLASH_Erase(address);
        address += 1024;
    }
}

static void UART_ISR(void)
{
    char data;
    UART_ReadByte(LPUART2, &data);
    CircularQueue_Enqueue(&queue, data);
}

static void Serial_Printf(char *format, ...)
{
    char buffer[100];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        UART_WriteByte(LPUART2, buffer[i]);
    }
}
