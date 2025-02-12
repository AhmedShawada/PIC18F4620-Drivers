/* 
 * File:   application.c
 * Author: ahmed.shawada
 * https://www.linkedin.com/in/ahmed-shawada/
 * Created on August 31, 2024, 12:32 AM
 */


#include "application.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/ADC/hal_adc.h"

uint16 adc_res_1, adc_res_2, adc_res_3, adc_res_4;
adc_conf_t adc_1 = 
{
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN0,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED            
};

adc_conf_t adc_2 = 
{
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN1,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED            
};

adc_conf_t adc_3 = 
{
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN2,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED            
};

adc_conf_t adc_4 = 
{
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN3,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED            
};
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();

    
    while (1)
    {
        ret = ADC_GetConversion_Blocking(&adc_1,ADC_CHANNEL_AN0,&adc_res_1);
        ret = ADC_GetConversion_Blocking(&adc_2,ADC_CHANNEL_AN1,&adc_res_2);
        ret = ADC_GetConversion_Blocking(&adc_3,ADC_CHANNEL_AN2,&adc_res_3);
        ret = ADC_GetConversion_Blocking(&adc_4,ADC_CHANNEL_AN3,&adc_res_4);
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = ADC_Init(&adc_1);
    ret = ADC_Init(&adc_2);
    ret = ADC_Init(&adc_3);
    ret = ADC_Init(&adc_4);
  
}
