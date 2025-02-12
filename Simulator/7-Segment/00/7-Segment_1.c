/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 28, 2024, 12:18 AM
 */

#include "application.h"

uint8 segment_common_anode [10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
uint8 segment_common_cathod[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
uint8 ones = 0;
uint8 tens = 0; 


   
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    
    

    while(1)
    {

        for(tens = 0; tens<10; tens++)
        {
            ret = gpio_port_write_logic(PORTC_INDEX,segment_common_anode [tens]);

            for(ones = 0; ones<10; ones++)
            {
             ret = gpio_port_write_logic(PORTD_INDEX,segment_common_cathod[ones]);
            __delay_ms(150);
            }
         }

    }
        
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = gpio_port_direction_intialize(PORTC_INDEX,0x80);
    ret = gpio_port_direction_intialize(PORTD_INDEX,0x80);
    ret = gpio_port_write_logic(PORTC_INDEX,segment_common_anode [0]);
    ret = gpio_port_write_logic(PORTD_INDEX,segment_common_cathod[0]);
}

