#ifndef SENSOR_SERVICE_H
#define SENSOR_SERVICE_H

#include <stdint.h>
#include "my_adc.h"
#include "my_lpit.h"
#define ADC_TEMP_CHANNEL 26
#define ADC_CHANNEL 0

#define VREF_MV 5000       // = Vdda(mV)
#define VTEMP25_MV 730     // Temp sensor voltage at 2.7 to 5.5 V 25 °C
#define TEMP_SLOPE 1.492    // Temp sensor slope at 2.7 to 5.5 V
#define SIZE_RECORD 10 //  1 record / 1ms


void TEMP_init(void);
float TEMP_getAvrValue(void);
float convertADCToCelsius(uint32_t adc_value);
void adc_callback(uint32_t adcValue);
uint32_t calculateAverageTemperature(void);

#endif /*SENSOR_SERVICE_H*/
