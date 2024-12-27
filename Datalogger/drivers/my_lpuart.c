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


static LPUART_CallBackType lpuart0Callback =0;
static LPUART_CallBackType lpuart1Callback =0;
static LPUART_CallBackType lpuart2Callback =0;
static void enableLPUARTClock(LPUART_Type *base)
{
    if (base == LPUART0) {
    	CLOCK_enable(PCC_LPUART0_INDEX);
    } else if (base == LPUART1) {
    	CLOCK_enable(PCC_LPUART1_INDEX);
    } else if (base == LPUART2) {
    	CLOCK_enable(PCC_LPUART2_INDEX);
    }
}


// Function to configure the pins for UART operation
static void setupPinPort(LPUART_Type *base)
{
    if (base == LPUART0) {
        // Enable clock for PORTB
        PCC->CLKCFG[PCC_PORTB_INDEX] |= PCC_CLKCFG_CGC(1);
        PORTB->PCR[0] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART0 RX
        PORTB->PCR[1] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART0 TX
    } else if (base == LPUART1) {
        // Enable clock for PORTC
        PCC->CLKCFG[PCC_PORTC_INDEX] |= PCC_CLKCFG_CGC(1);
        PORTC->PCR[6] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART1 RX
        PORTC->PCR[7] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART1 TX
    } else if (base == LPUART2) {
        // Enable clock for PORTD
        PCC->CLKCFG[PCC_PORTD_INDEX] |= PCC_CLKCFG_CGC(1);
        PORTD->PCR[6] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART2 RX
        PORTD->PCR[7] |= PORT_PCR_MUX(2);  // MUX = ALT2, UART2 TX
    } else {
        // error
    }
}


void LPUART_begin(LPUART_Type *base, uint32_t baudRate)
{

    // Enable clock for the UART peripheral
    enableLPUARTClock(base);

    // Setup the pin port for UART operation
    setupPinPort(base);

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
void LPUART_enableRxInterrupts(LPUART_Type *base,LPUART_CallBackType callback)
{
	// Enable both receiver and transmitter interrupts
	base->CTRL |= (LPUART_CTRL_RIE_MASK);

    if(base == LPUART0){
        NVIC_EnableIRQ(LPUART0_IRQn);
        lpuart0Callback = callback;
    }else if(base == LPUART1){
    	NVIC_EnableIRQ(LPUART1_IRQn);
    	 lpuart1Callback = callback;
    }else{
    	NVIC_EnableIRQ(LPUART2_IRQn);
    	lpuart2Callback = callback;
    }


}


// Function to send a string via UART
void LPUART_write(LPUART_Type *base,uint8_t data)
{
    while((base->STAT & LPUART_STAT_TDRE_MASK)>>LPUART_STAT_TDRE_SHIFT==0);  // Wait for TX buffer to be empty
    base->DATA = data;  // Send character

}

// Function to check if data is available to read from UART
uint8_t LPUART_available(LPUART_Type *base)
{
    return (base->STAT & LPUART_STAT_RDRF_MASK) != 0;  //!=0 can read
}

uint8_t LPUART_readByte(LPUART_Type *base)
{
	uint8_t data = base->DATA;
	return data;
}

void LPUART0_IRQHandler(void){
	if(lpuart0Callback != 0){
		lpuart0Callback();
	}
}
void LPUART1_IRQHandler(void){
	if(lpuart1Callback != 0){
			lpuart0Callback();
		}
}
void LPUART2_IRQHandler(void){
	if(lpuart2Callback != 0){
			lpuart0Callback();
		}
}
