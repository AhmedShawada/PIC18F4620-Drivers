/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 28, 2024, 12:18 AM
 */

#include "application.h"

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


chr_lcd_8bits_t lcd_2 =
{
    .lcd_rs.port = PORTC_INDEX,
    .lcd_rs.pin = GPIO_PIN6,
    .lcd_rs.logic = GPIO_LOW,
    .lcd_rs.direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_en.port = PORTC_INDEX,
    .lcd_en.pin = GPIO_PIN7,
    .lcd_en.direction = GPIO_LOW,
    .lcd_en.direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[0].port = PORTD_INDEX,
    .lcd_data[0].pin = GPIO_PIN0,
    .lcd_data[0].logic = GPIO_LOW,
    .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[1].port = PORTD_INDEX,
    .lcd_data[1].pin = GPIO_PIN1,
    .lcd_data[1].logic = GPIO_LOW,
    .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[2].port = PORTD_INDEX,
    .lcd_data[2].pin = GPIO_PIN2,
    .lcd_data[2].logic = GPIO_LOW,
    .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[3].port = PORTD_INDEX,
    .lcd_data[3].pin = GPIO_PIN3,
    .lcd_data[3].logic = GPIO_LOW,
    .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[4].port = PORTD_INDEX,
    .lcd_data[4].pin = GPIO_PIN4,
    .lcd_data[4].logic = GPIO_LOW,
    .lcd_data[4].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[5].port = PORTD_INDEX,
    .lcd_data[5].pin = GPIO_PIN5,
    .lcd_data[5].logic = GPIO_LOW,
    .lcd_data[5].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[6].port = PORTD_INDEX,
    .lcd_data[6].pin = GPIO_PIN6,
    .lcd_data[6].logic = GPIO_LOW,
    .lcd_data[6].direction = GPIO_DIRECTION_OUTPUT,
    
    .lcd_data[7].port = PORTD_INDEX,
    .lcd_data[7].pin = GPIO_PIN7,
    .lcd_data[7].logic = GPIO_LOW,
    .lcd_data[7].direction = GPIO_DIRECTION_OUTPUT
};


const uint8 customChar_0[] = {0x0E,0x0A,0x1B,0x11,0x11,0x11,0x1F,0x00};
const uint8 customChar_1[] = {0x0E,0x0A,0x1B,0x11,0x11,0x1F,0x1F,0x00};
const uint8 customChar_2[] = {0x0E,0x0A,0x1B,0x11,0x1F,0x1F,0x1F,0x00};
const uint8 customChar_3[] = {0x0E,0x0A,0x1B,0x1F,0x1F,0x1F,0x1F,0x00};
const uint8 customChar_4[] = {0x0E,0x0E,0x1F,0x1F,0x1F,0x1F,0x1F,0x00};

const uint8 customChar_5[] = {0x04,0x0A,0x04,0x1F,0x04,0x04,0x0A,0x11};
  


uint8 lcd_conter = ZERO_INIT;
uint8 lcd_counter_txt = ZERO_INIT;
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
   
    
    while(1)
    {
        ret = lcd_8bits_send_custom_char(&lcd_2,1,20,customChar_4,0);
        ret = lcd_4bits_send_custom_char(&lcd_1,1,20,customChar_4,1);
        for(lcd_conter = 1;lcd_conter<=20;lcd_conter++)
        {
          ret = lcd_4bits_send_custom_char(&lcd_1,4,lcd_conter,customChar_5,2);
          __delay_ms(50);
          ret = lcd_4bits_send_char_data_pos(&lcd_1,4,lcd_conter,' ');
          if(20 == lcd_conter)
              
          {
              for(lcd_counter_txt = 1;lcd_counter_txt<=20;lcd_counter_txt++)
              {
                ret = lcd_8bits_send_custom_char(&lcd_2,4,lcd_counter_txt,customChar_5,2);
                __delay_ms(50);
                ret = lcd_8bits_send_char_data_pos(&lcd_2,4,lcd_counter_txt,' '); 
              }
          }
        }
    } 
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = lcd_4bits_intialize(&lcd_1);
    ret = lcd_8bits_intialize(&lcd_2);
}

