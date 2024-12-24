#include "my_lpit.h"
#include "my_clock.h"
#include <stddef.h>
/*! Macro to convert a microsecond period to raw count value */
#define f CLOCK_getFreq()
#define USEC_TO_COUNT(us, clockFreqInHz) (uint64_t)(((uint64_t)(us) * (clockFreqInHz)) / 1000000U)

volatile LPIT_CallBackType _lpitCallback ;
volatile uint8_t gChannel;
void LPIT_init(){
	CLOCK_enable(PCC_LPIT0_INDEX);
	//• Reset the timer channels and registers;
	LPIT0->MCR |= LPIT_MCR_SW_RST(1);
	LPIT0->MCR &= ~LPIT_MCR_SW_RST_MASK;
	//• Setup timer operation in debug and doze modes
	// and enable the module;
	LPIT0->MCR |= LPIT_MCR_M_CEN(1);

	// disable interrupt
	LPIT0->MIER &= ~ LPIT_MIER_TIE0_MASK;

}


void LPIT_SetPriority(uint8_t priority){
	__NVIC_SetPriority(LPIT0_IRQn, priority);
}

void LPIT_setCallBack(LPIT_CallBackType function){
	//	//• Enable channel0 interrupt;
	LPIT0->MIER |= LPIT_MIER_TIE0(1);
	NVIC->ISER[0] |= (1 << 22);
	_lpitCallback = function;
}
void LPIT0_IRQHandler(void){
	// clear interrupt flag
	LPIT0->MSR = LPIT_MSR_TIF0(1);
	if(NULL != _lpitCallback){
		_lpitCallback();
	}
}
void LPIT_startTimer(uint8_t channel){
	gChannel = channel;
	// CTRL[channel]_EN
	LPIT0->SETTEN |= LPIT_SETTEN_SET_T_EN_0(1);
	LPIT0->CHANNEL[gChannel].TCTRL |= LPIT_TCTRL_T_EN(1);
}

void LPIT_delay(uint32_t microSecond){

	/* f = 1Mhz = 1 000 000 hz
	 * delay time = 0.2s = T *(reload-1) = (reload-1)/f
	 * reload = [0.2]*1000000+1 = 200 001
	 */
	LPIT0->CHANNEL[gChannel].TCTRL &= ~LPIT_TCTRL_MODE_MASK;
	LPIT0->CHANNEL[gChannel].TVAL = USEC_TO_COUNT(microSecond,f)*1000 +1;


}
