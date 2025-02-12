/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 28, 2024, 12:18 AM
 */

#include "application.h"
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
            ret = gpio_port_write_logic(PORTC_INDEX,tens);

            for(ones = 0; ones<10; ones++)
            {
             ret = gpio_port_write_logic(PORTD_INDEX,ones);
            __delay_ms(150);
            }
         }

    }
        
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = gpio_port_direction_intialize(PORTC_INDEX,0xF0);
    ret = gpio_port_direction_intialize(PORTD_INDEX,0xF0);
    ret = gpio_port_write_logic(PORTC_INDEX,0);
    ret = gpio_port_write_logic(PORTD_INDEX,0);
    
}

