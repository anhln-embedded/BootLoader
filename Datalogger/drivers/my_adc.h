/*
 * my_adc.h
 *
 *  Created on: Dec 4, 2024
 *      Author: nvd
 */

#ifndef MY_ADC_H_
#define MY_ADC_H_
#include "MKE16Z4.h"
typedef void (*ADC_CallBackType)(uint32_t adc_value);

void ADC_ReadAsync(ADC_CallBackType _adcCallback);
void ADC_Init(uint8_t adc_channel, uint32_t adhc_value);
uint16_t ADC_ReadSysc();
void ADC_SetPriority(uint8_t priority);
#endif /* MY_ADC_H_ */









