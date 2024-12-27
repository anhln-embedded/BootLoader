/*
 * my_adc.c
 *
 *  Created on: Dec 4, 2024
 *      Author: nvd
 */
#include <stddef.h>
#include "my_adc.h"
#include "my_clock.h"
uint8_t ADC_CHANNEL;
uint32_t ADC_TEMP_CHANNEL;
volatile ADC_CallBackType _adcCallback = NULL;

void ADC_Init(uint8_t adc_channel, uint32_t adhc_value)
{
	ADC_TEMP_CHANNEL = adhc_value;
	ADC_CHANNEL = adc_channel;

	CLOCK_enable(PCC_ADC0_INDEX);


	ADC0->CFG1 &= ~ADC_CFG1_ADICLK_MASK;	// ALT CLK 1

}

uint16_t ADC_ReadSysc(){
	ADC0->SC1[ADC_CHANNEL] = (ADC0->SC1[ADC_CHANNEL] & ~ADC_SC1_ADCH_MASK) | ADC_SC1_ADCH(ADC_TEMP_CHANNEL);
	while((ADC0->SC1[ADC_CHANNEL] & ADC_SC1_COCO_MASK) == 0);
	return ADC0->R[ADC_CHANNEL];
}
void ADC_ReadAsync(ADC_CallBackType callback)
{
	//NVIC
	__NVIC_EnableIRQ(ADC0_IRQn);
	// Interrupt

	ADC0->SC1[ADC_CHANNEL] |= ADC_SC1_AIEN(1);
	// set channel ADC
	ADC0->SC1[ADC_CHANNEL] = (ADC0->SC1[ADC_CHANNEL] & ~ADC_SC1_ADCH_MASK) | ADC_SC1_ADCH(ADC_TEMP_CHANNEL);
	//
	_adcCallback = callback;
}

void ADC0_IRQHandler(void){
	uint32_t value = ADC0->R[ADC_CHANNEL];
	if(_adcCallback != NULL) _adcCallback(value);

	ADC0->SC1[ADC_CHANNEL] = (ADC0->SC1[ADC_CHANNEL] & ~ADC_SC1_ADCH_MASK) | ADC_SC1_ADCH(ADC_TEMP_CHANNEL);
}
void ADC_SetPriority(uint8_t priority){
	__NVIC_SetPriority(ADC0_IRQn, priority);
}

