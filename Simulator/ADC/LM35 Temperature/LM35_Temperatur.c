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

uint16 LM35_RES_1 = 0, LM35_RES_2 = 0, lm35_res_1_Celsius_mv = 0, lm35_res_2_Celsius_mv = 0;
uint8  adc_res1_txt[7], adc_res2_txt[7];
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

dc_motor_t dc_motor_1 = {
    .dc_motor_pin[0].port = PORTD_INDEX,
    .dc_motor_pin[0].pin = GPIO_PIN0,
    .dc_motor_pin[0].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[0].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[1].port = PORTD_INDEX,
    .dc_motor_pin[1].pin = GPIO_PIN1,
    .dc_motor_pin[1].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[1].direction = GPIO_DIRECTION_OUTPUT
};

dc_motor_t dc_motor_2 = {
    .dc_motor_pin[0].port = PORTD_INDEX,
    .dc_motor_pin[0].pin = GPIO_PIN2,
    .dc_motor_pin[0].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[0].direction = GPIO_DIRECTION_OUTPUT,
    .dc_motor_pin[1].port = PORTD_INDEX,
    .dc_motor_pin[1].pin = GPIO_PIN3,
    .dc_motor_pin[1].logic = DC_MOTOR_OFF_STATUS,
    .dc_motor_pin[1].direction = GPIO_DIRECTION_OUTPUT
};


int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 3, "TEMPERATURE TEST");
    __delay_ms(500);
    ret = lcd_4bits_send_command(&lcd_1, _LCD_CLEAR);
    
    ret = lcd_4bits_send_string_pos(&lcd_1, 1, 1, "TEMP1 : ");
    ret = lcd_4bits_send_string_pos(&lcd_1, 2, 1, "TEMP2 : ");
 
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
        else { /* Nothing */ }
        
        lm35_res_1_Celsius_mv = LM35_RES_1 * 4.88f ;  /* 5000(mv) / 1024(10-Bit) => 4.88 */
        lm35_res_2_Celsius_mv = LM35_RES_2 * 4.88f ;
        
        lm35_res_1_Celsius_mv /= 10;  /* The sensitivity of LM35 is 10 mV/degree Celsius */
        lm35_res_2_Celsius_mv /= 10; 
        
        
        ret = convert_uint16_to_string(lm35_res_1_Celsius_mv,adc_res1_txt);
        ret = convert_uint16_to_string(lm35_res_2_Celsius_mv,adc_res2_txt);

        
        ret = lcd_4bits_send_string_pos(&lcd_1, 1, 8,adc_res1_txt);
        ret = lcd_4bits_send_string_pos(&lcd_1, 2, 8,adc_res2_txt);
        
        if(lm35_res_1_Celsius_mv >= 30)
        {
            ret = dc_motor_move_right(&dc_motor_1);
        }
        else
        {
            ret = dc_motor_stop(&dc_motor_1);
        }
        
        if(lm35_res_2_Celsius_mv >= 35)
        {
            ret = dc_motor_move_right(&dc_motor_2);
        }
        else
        {
            ret = dc_motor_stop(&dc_motor_2);
        }
 
    }
    
    return (EXIT_SUCCESS);

}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK;
    ret = ADC_Init(&adc_1);
    ret = lcd_4bits_intialize(&lcd_1);
    ret = dc_motor_intialize(&dc_motor_1);
    ret = dc_motor_intialize(&dc_motor_2);
}

void ADC_Defult_Interrupyt(void)
{
    std_ReturnType ret = E_NOT_OK;
    if(0 == ADC_REQ)
    {
        ret = ADC_GetConversionResult(&adc_1, &LM35_RES_1);
        ADC_REQ = 1;
    }
    else if(1 == ADC_REQ)
    {
        ret = ADC_GetConversionResult(&adc_1, &LM35_RES_2);
        ADC_REQ = 0;
    }

    else { /* Nothing */ }
    
}
