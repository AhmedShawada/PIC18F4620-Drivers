/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 28, 2024, 12:18 AM
 */

#include "application.h"
uint8 Number = 0;
uint8 Number_BCD = 0; 


   
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    while(1)
    {
        Number_BCD = (((uint8)Number%10) | (((uint8)Number/10) << 4));
        ret = gpio_port_write_logic(PORTC_INDEX,Number_BCD);
        if (99 == Number)
         {      
            Number = 0;
         }
        Number++;
        __delay_ms(150);
    } 
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = gpio_port_direction_intialize(PORTC_INDEX,0x00);
    ret = gpio_port_write_logic(PORTC_INDEX,0);
    
}

