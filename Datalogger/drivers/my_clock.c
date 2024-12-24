#include "my_clock.h"

void CLOCK_enable(uint8_t index)
{
	PCC->CLKCFG[index] &= ~PCC_CLKCFG_CGC_MASK;
	PCC->CLKCFG[index] &= ~PCC_CLKCFG_PCS_MASK;
	PCC->CLKCFG[index] |= PCC_CLKCFG_PCS(2);
	SCG->SIRCDIV &= ~SCG_SIRCDIV_SIRCDIV2_MASK;
	SCG->SIRCDIV |= SCG_SIRCDIV_SIRCDIV2(4);// clock=8 / 8 div = 1Mhz
	// enable clock/ async clock
	PCC->CLKCFG[index] |= PCC_CLKCFG_CGC(1);
}
uint32_t CLOCK_getFreq(void){
	return 1000000;
}
