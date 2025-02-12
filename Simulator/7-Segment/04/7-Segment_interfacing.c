/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 28, 2024, 12:18 AM
 */

#include "application.h"
segment_t seg1 = 
{
    .segment_pins[0].pin = GPIO_PIN0,
    .segment_pins[0].port = PORTC_INDEX,
    .segment_pins[0].logic = GPIO_LOW,
    .segment_pins[0].direction = GPIO_DIRECTION_OUTPUT,
    
    .segment_pins[1].pin = GPIO_PIN1,
    .segment_pins[1].port = PORTC_INDEX,
    .segment_pins[1].logic = GPIO_LOW,
    .segment_pins[1].direction = GPIO_DIRECTION_OUTPUT,
    
    .segment_pins[2].pin = GPIO_PIN2,
    .segment_pins[2].port = PORTC_INDEX,
    .segment_pins[2].logic = GPIO_LOW,
    .segment_pins[2].direction = GPIO_DIRECTION_OUTPUT,
    
    .segment_pins[3].pin = GPIO_PIN3,
    .segment_pins[3].port = PORTC_INDEX,
    .segment_pins[3].logic = GPIO_LOW,
    .segment_pins[3].direction = GPIO_DIRECTION_OUTPUT,
    .segment_type = SEGMENT_COMMON_ANODE
};


uint8 number  = 0;
uint8 counter = 0;


   
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    while(1)
    {
        for(number = 0; number <= 9; number++)
        {
          ret = seven_segment_write_number(&seg1,number);
          __delay_ms(500);
        }
   
    } 
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = seven_segment_intialize(&seg1);
}

