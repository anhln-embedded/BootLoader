/*
 * bootloader.c
 *
 *  Created on: Dec 21, 2024
 *      Author: hnaco
 */

#include "bootloader.h"


LPUART_InitStruct_t LPUART_InitStruct = {
    .BaudRate = 9600,
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

uint8_t Flash_SrecLine(SrecLine_t *Data)
{
    uint32_t dataFlash = 0;
    for (int i = 0; i < (Data->u8ByteCount - 4); i += 4)
    {
        dataFlash = (Data->pData[i + 3] << 24) | (Data->pData[i + 2] << 16) | (Data->pData[i + 1] << 8) | Data->pData[i];
        FLASH_Write(Data->u32Address + i, dataFlash);
    }
    return 1;
}

void UART_ISR(void)
{
    char data;
    UART_ReadByte(LPUART0, &data);
    CircularQueue_Enqueue(&queue, data);
}

void Serial_Printf(char *format, ...)
{
    char buffer[100];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        UART_WriteByte(LPUART0, buffer[i]);
    }
}

uint8_t var_test1 = 0;
uint8_t var_test2 = 0;
uint8_t var_test3 = 0;
uint8_t var_test4 = 0;


uint8_t BOOT_MODE;


void BootInit(void)
{
    __enable_irq();
    /* Enable clock for PORTB */
    EnableClockForPort(PCC_PORTB_INDEX);
    PORT_Pin_Init(PORT_INS_B, 0, &config);
    PORT_Pin_Init(PORT_INS_B, 1, &config);
    UART_Init(LPUART0, &LPUART_InitStruct);
    UART_RegisterHandler(LPUART0, UART_ISR);
    Serial_Printf("address: 0x%08x\n", &BOOT_MODE);

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

void BootFirmwareUpdate(void)
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
                    BootJumpToApplication(APPLICATION_ADRESS_1);
                }
                else
                {
                    Serial_Printf("Flash\n");
                    if (*(uint32_t *)(Data.u32Address) != 0xFFFFFFFF)
                    {
                        Serial_Printf("Erase\n");
                        // FLASH_Erase(Data.u32Address);
                    }
                    Flash_SrecLine(&Data);
                }
            }
            else
            {
                Serial_Printf("Error\n");
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
