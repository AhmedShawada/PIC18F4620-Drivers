/* 
 * File:   application.c
 * Author: ahmed.shawada
 * https://www.linkedin.com/in/ahmed-shawada/
 * Created on November 28, 2024, 8:07 PM
 */



#include "application.h"

#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer0/hal_timer0.h"
#include "MCAL_Layer/Timer1/hal_timer1.h"
#include "MCAL_Layer/Timer2/hal_timer2.h"
#include "MCAL_Layer/Timer3/hal_timer3.h"
#include "MCAL_Layer/CCP/hal_ccp.h"
#include "MCAL_Layer/USART/hal_usart.h"
#include "MCAL_Layer/I2C/hal_i2c.h"

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

ultrasonic_t UltraSonic;
uint16 Distance;
uint16 Distance_txt[6];
int main()
{  
    std_ReturnType ret = E_NOT_OK;
    
    UltraSonic.Trigger_Pin.port = PORTD_INDEX;
    UltraSonic.Trigger_Pin.pin = GPIO_PIN0;
    UltraSonic.Trigger_Pin.direction = GPIO_DIRECTION_OUTPUT;  
    UltraSonic.Trigger_Pin.logic = GPIO_LOW;
    UltraSonic.Echo_Pin.port = PORTD_INDEX;
    UltraSonic.Echo_Pin.pin = GPIO_PIN1;
    UltraSonic.Echo_Pin.direction = GPIO_DIRECTION_INPUT;
    UltraSonic.Echo_Pin.logic = GPIO_LOW;
    
    application_intialize();
    ret = lcd_4bits_send_string_pos(&lcd_1,1,1,"Distance: ");
   
    
    while (1)
    { 
        ret = Ultrasonic_Calculate_Distance(&UltraSonic,&Distance);
        ret = convert_uint16_to_string(Distance,Distance_txt);
        ret = lcd_4bits_send_string_pos(&lcd_1,1,11,Distance_txt);
        
        __delay_ms(50);
    }
    
    return (EXIT_SUCCESS);
}

void application_intialize(void) 
{
    std_ReturnType ret = E_NOT_OK; 
    ret = Ultrasonic_Init(&UltraSonic);
    ret = lcd_4bits_intialize(&lcd_1);
    

}


