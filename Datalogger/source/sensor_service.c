#include "sensor_service.h"
#include "MKE16Z4.h"

static uint32_t record[SIZE_RECORD];
static uint8_t recordIndex = 0;

void TEMP_init(){
	ADC_Init(ADC_CHANNEL,ADC_TEMP_CHANNEL);
	ADC_ReadAsync(adc_callback);
}
float TEMP_getAvrValue(){
	uint32_t avgADC = calculateAverageTemperature();
	return convertADCToCelsius(avgADC);
}
//void initTemperatureReaderSync(uint32_t mTime, uint32_t *result){
//	ADC_Init(ADC_CHANNEL,ADC_TEMP_CHANNEL);
//	LPIT_init();
//	LPIT_delay(mTime);
//	LPIT_setCallBack(function);
//	record = result;
//}
//void LPIT_readADCCallBack(){
//	*record = ADC_ReadSysc();
//}
float convertADCToCelsius(uint32_t adc_value){
	// adc to voltage
	float Vtemp_mV = (float)VREF_MV * adc_value / 4096;
	// vt => °C
	return (25000 - ((Vtemp_mV - VTEMP25_MV) / TEMP_SLOPE))/1000;
}

void adc_callback(uint32_t adcValue){
	record[recordIndex] = adcValue;
	recordIndex = (recordIndex + 1) % SIZE_RECORD;
}
uint32_t calculateAverageTemperature(){
	uint32_t sum = 0;
	for (uint32_t i = 0; i < SIZE_RECORD; i++) {
		sum += record[i];
	}
	return sum / SIZE_RECORD;
}
