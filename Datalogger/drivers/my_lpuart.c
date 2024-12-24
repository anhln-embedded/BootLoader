/*
 * my_uart.c
 *
 *  Created on: Dec 21, 2024
 *      Author: nvd
 */
#include <string.h>
#include <stdlib.h>
#include "my_lpuart.h"
#include "my_clock.h"
#define LPUART_CLOCK CLOCK_getFreq()  // Clock frequency for LPUART

static LPUART_Type *gBase;
void LPUART_begin(LPUART_Type *base, uint32_t baudRate)
{
    gBase = base;

    // Enable clock for the UART peripheral
    enableLPUARTClock();

    // Setup the pin port for UART operation
    setupPinPort();

    // Calculate the baud rate setting
    uint16_t sbr = (uint16_t)((LPUART_CLOCK) / (baudRate * 16));
    base->BAUD = LPUART_BAUD_SBR(sbr) | LPUART_BAUD_OSR(15);  // Set baud rate and oversampling ratio
    /*
    * OSR=15: Over sampling ratio = 15+1=16
    * SBNS=0: One stop bit
    * BOTHEDGE=0: receiver samples only on rising edge
    * M10=0: Rx and Tx use 7 to 9 bit data characters
    * RESYNCDIS=0: Resync during rec'd data word supported
    * LBKDIE, RXEDGIE=0: interrupts disabled
    * TDMAE, RDMAE, TDMAE=0: DMA requests disabled
    * MAEN1, MAEN2, MATCFG=0: Match disabled
    */
  // Enable receiver and transmitter
    base->CTRL = LPUART_CTRL_RE_MASK | LPUART_CTRL_TE_MASK;
    /*
     * RE=1: Receiver enabled
     * TE=1: Transmitter enabled
     * PE,PT=0: No hw parity generation or checking
     * M7,M,R8T9,R9T8=0: 8-bit data char
     * DOZEEN=0: enabled in Doze mode
     * ORIE,NEIE,FEIE,PEIE,TIE,TCIE,RIE,ILIE,MA1IE,MA2IE=0: no IRQ
     * TxDIR=0: TxD pin is input if in single-wire mode
     * TXINV=0: Transmit data not inverted
     * RWU,WAKE=0: normal operation; rcvr not in standby
     * IDLCFG=0: one idle character
     * ILT=0: Idle char bit count starts after start bit
     * SBK=0: Normal transmitter operation - no break char
     * LOOPS,RSRC=0: no loop back
     */

    // transmit tie LPUART Control Register (CTRL)

}
void LPUART_enableInterrupts()
{
	// Enable both receiver and transmitter interrupts
	gBase->CTRL |= (LPUART_CTRL_RIE_MASK | LPUART_CTRL_TIE_MASK);

    if(gBase == LPUART0){
        NVIC_EnableIRQ(LPUART0_IRQn);
    }else if(gBase == LPUART1){
    	NVIC_EnableIRQ(LPUART1_IRQn);
    }else{
    	NVIC_EnableIRQ(LPUART2_IRQn);
    }
}
void LPUART_Callback(){
	 // Clear RX interrupt flag (RDRF - Receiver Data Register Full)
	    if (gBase->STAT & LPUART_STAT_RDRF_MASK) {
	        // Reading from the DATA register will clear the RDRF flag
	        volatile uint8_t dummy = gBase->DATA;  // Clear the flag by reading DATA
	    }

	    // Clear TX interrupt flag (TDRE - Transmitter Data Register Empty)
	    if (gBase->STAT & LPUART_STAT_TDRE_MASK) {
	        // No action needed to clear TDRE flag, but reading or writing to DATA register will reset it.
	        // For non-blocking transmit, the flag will clear automatically once the data is shifted out.
	        volatile uint8_t dummy = gBase->DATA;  // Clear the flag by reading DATA (optional)
	    }
}
void enableLPUARTClock()
{
    if (gBase == LPUART0) {
    	CLOCK_enable(PCC_LPUART0_INDEX);
    } else if (gBase == LPUART1) {
    	CLOCK_enable(PCC_LPUART1_INDEX);
    } else if (gBase == LPUART2) {
    	CLOCK_enable(PCC_LPUART2_INDEX);
    }
}


// Function to configure the pins for UART operation
void setupPinPort()
{
    if (gBase == LPUART0) {
        // Enable clock for PORTB
        PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1);
        PORTB->PCR[0] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART0 RX
        PORTB->PCR[1] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART0 TX
    } else if (gBase == LPUART1) {
        // Enable clock for PORTC
        PCC->CLKCFG[PCC_PORTC_INDEX] |= PCC_CLKCFG_CGC(1);
        PORTC->PCR[6] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART1 RX
        PORTC->PCR[7] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART1 TX
    } else if (gBase == LPUART2) {
        // Enable clock for PORTD
        PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC(1);
        PORTD->PCR[6] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART2 RX
        PORTD->PCR[7] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART2 TX
    } else {
        // error
    }
}


// Function to send a string via UART
void LPUART_print(char *data)
{
    uint32_t i = 0;
    while (data[i] != '\0') {
    	while((gBase->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT==0);  // Wait for TX buffer to be empty
        gBase->DATA = data[i];  // Send character
        i++;
    }
}

// Function to send a string followed by a newline
void LPUART_println(char *data)
{
    LPUART_print(data);  // Send the string
    LPUART_print("\r\n"); // Send newline and carriage return
}
// Function to check if data is available to read from UART
uint8_t LPUART_available()
{
    return (gBase->STAT & LPUART_STAT_RDRF_MASK) != 0;  //!=0 can read
}
// Function to read a string from UART
char *LPUART_readString(uint32_t size)
{
    char *buffer = (char *)malloc(size);
    if (buffer == NULL) {
        return NULL; // if memory allocation fails
    }

    uint32_t i = 0;
    while (i < size - 1) {
    	while((gBase->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT==0);;//wait data
        buffer[i] = gBase->DATA;
        if (buffer[i] == '\n') {
            break;
        }
        i++;
    }
    buffer[i] = '\0';
    return buffer;
}
char *LPUART_readUntil(char endChar) {
    uint32_t bufferSize = 128;
    char *buffer = (char *)malloc(bufferSize);
    if (buffer == NULL) {
        return NULL; // Return NULL if malloc fails
    }

    uint32_t i = 0;

    // Wait for data to be received and check LPUART available
    while (1) {
        // Wait for data if nothing available
    	while((gBase->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT==0);

        char receiveChar = gBase->DATA;

        if (receiveChar == endChar) {
            break;  // Break the loop when endChar is received
        }

        // Store the received character into buffer
        buffer[i++] = receiveChar;

        // Reallocate buffer if it's full
        if (i >= bufferSize) {
            bufferSize *= 2; // Double the buffer size
            char *newBuffer = (char *)realloc(buffer, bufferSize);
            if (newBuffer == NULL) {
                free(buffer); // Free the old buffer to avoid memory leak
                return NULL;  // Return NULL if realloc fails
            }
            buffer = newBuffer; // Update buffer to the new memory location
        }
    }

    // Null-terminate the string
    buffer[i] = '\0';
    return buffer;
}
// Function to read multiple bytes from UART
uint32_t LPUART_readBytes(uint8_t *buffer, uint32_t size)
{
    uint32_t bytesRead = 0;
    while (bytesRead < size) {
        //while ((gBase->STAT & LPUART_STAT_RDRF_MASK) == 0);  // Wait for data to be received
        buffer[bytesRead] = gBase->DATA;
        bytesRead++;
    }
    return bytesRead;
}

void LPUART0_IRQHandler(void){
	LPUART_Callback();
}
void LPUART1_IRQHandler(void){
	LPUART_Callback();
}
void LPUART2_IRQHandler(void){
	LPUART_Callback();
}

