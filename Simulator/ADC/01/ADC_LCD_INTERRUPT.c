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

uint16 adc_res_1 = 0, adc_res_2 = 0, adc_res_3 = 0, adc_res_4 = 0;
uint8  adc_res1_txt[6], adc_res2_txt[6], adc_res3_txt[6], adc_res4_txt[6];
uint8 ADC_REQ = 0;

void ADC_Defult_Interrupyt(void);


adc_conf_t adc_1 = 
{
    .ADC_InterruptHandler = ADC_Defult_Interrupyt,
    .priority = INTERRUPT_HIGH_PRIORITY,
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN0,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED            
};


chr_lcd_4bits_t lcd_1 =
{
    .lcd_rs.port = PORTC_INDEX,
    .lcd_rs.pin = GPIO_PIN0,
    .lcd_rs.logic = GPIO_LOW,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_en.port = PORTC_INDEX,
    .lcd_en.pin = GPIO_PIN1,
    .lcd_en.direction = GPIO_LOW,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[0].port = PORTC_INDEX,
    .lcd_data[0].pin = GPIO_PIN2,
    .lcd_data[0].logic = GPIO_LOW,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[1].port = PORTC_INDEX,
    .lcd_data[1].pin = GPIO_PIN3,
    .lcd_data[1].logic = GPIO_LOW,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[2].port = PORTC_INDEX,
    .lcd_data[2].pin = GPIO_PIN4,
    .lcd_data[2].logic = GPIO_LOW,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[3].port = PORTC_INDEX,
    .lcd_data[3].pin = GPIO_PIN5,
    .lcd_data[3].logic = GPIO_LOW,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT 
};


int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 6, "ADC TEST");
    __delay_ms(200);
    ret = lcd_4bits_send_command(&lcd_1, _LCD_CLEAR);
    
    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 1, "POT1 : ");
    ret = lcd_4bits_send_string_pos(&lcd_1, 2, 1, "POT2 : ");
    ret = lcd_4bits_send_string_pos(&lcd_1, 3, 1, "POT3 : ");
    ret = lcd_4bits_send_string_pos(&lcd_1, 4, 1, "POT4 : ");
    
    
    while (1)
    {
        
        
        if(0 == ADC_REQ)
        {
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN0);
        }
        else if(1 == ADC_REQ)
        {
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN1);
        }
        else if(2 == ADC_REQ)
        {
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN2);
        }
        else if(3 == ADC_REQ)
        {
            ret = ADC_StartConversion_Interrupt(&adc_1, ADC_CHANNEL_AN3);
        }
        else { /* Nothing */ }
        
        
        ret = convert_uint16_to_string(adc_res_1,adc_res1_txt);
        ret = convert_uint16_to_string(adc_res_2,adc_res2_txt);
        ret = convert_uint16_to_string(adc_res_3,adc_res3_txt);
        ret = convert_uint16_to_string(adc_res_4,adc_res4_txt);
        
        ret = lcd_4bits_send_string_pos(&lcd_1, 1, 8,adc_res1_txt);
        ret = lcd_4bits_send_string_pos(&lcd_1, 2, 8,adc_res2_txt);
        ret = lcd_4bits_send_string_pos(&lcd_1, 3, 8,adc_res3_txt);
        ret = lcd_4bits_send_string_pos(&lcd_1, 4, 8,adc_res4_txt);
        
        
        
        
        
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = ADC_Init(&adc_1);
    ret = lcd_4bits_intialize(&lcd_1);
}

void ADC_Defult_Interrupyt(void)
{
    std_ReturnType ret = E_NOT_OK;
    if(0 == ADC_REQ)
    {
        ret = ADC_GetConversionResult(&adc_1, &adc_res_1);
        ADC_REQ = 1;
    }
    else if(1 == ADC_REQ)
    {
        ret = ADC_GetConversionResult(&adc_1, &adc_res_2);
        ADC_REQ = 2;
    }
    else if(2 == ADC_REQ)
    {
        ret = ADC_GetConversionResult(&adc_1, &adc_res_3);
        ADC_REQ = 3;
    }
    else if(3 == ADC_REQ){
        ret = ADC_GetConversionResult(&adc_1, &adc_res_4);
        ADC_REQ = 0;
    }
    else { /* Nothing */ }
    
}
