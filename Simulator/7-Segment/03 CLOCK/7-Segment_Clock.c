/* 
 * File:   application.c
 * Author: Ahmed_Shawada
 *
 * Created on July 28, 2024, 12:18 AM
 */

#include "application.h"
uint8 hours = 23;
uint8 minutes = 59;
uint8 seconds = 59;
uint8 counter = 0;


   
int main()
{
    std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    while(1)
    {
        for(counter = 0; counter < 50; counter++)
        {
            ret = gpio_port_write_logic(PORTD_INDEX,0x01);
            ret = gpio_port_write_logic(PORTC_INDEX,(uint8)hours/10);
            __delay_us(3333);
            ret = gpio_port_write_logic(PORTD_INDEX,0x02);
            ret = gpio_port_write_logic(PORTC_INDEX,(uint8)hours%10);
            __delay_us(3333);
            ret = gpio_port_write_logic(PORTD_INDEX,0x04);
            ret = gpio_port_write_logic(PORTC_INDEX,(uint8)minutes/10);
            __delay_us(3333);
            ret = gpio_port_write_logic(PORTD_INDEX,0x08);
            ret = gpio_port_write_logic(PORTC_INDEX,(uint8)minutes%10);
            __delay_us(3333);
            ret = gpio_port_write_logic(PORTD_INDEX,0x10);
            ret = gpio_port_write_logic(PORTC_INDEX,(uint8)seconds/10);
            __delay_us(3333);
            ret = gpio_port_write_logic(PORTD_INDEX,0x20);
            ret = gpio_port_write_logic(PORTC_INDEX,(uint8)seconds%10);
            __delay_us(3333);
        }
        seconds++;
        if(60 == seconds)
        {
            minutes++;
            seconds = 0;
        }
        
        if(60 == minutes)
        {
          hours++;
          minutes = 0;
        }
        
        if(24 == hours)
        {
            hours = 0;
        }    
                
        
    } 
}
void application_intialize (void)
{
    std_ReturnType ret = E_NOT_OK;
    ret = gpio_port_direction_intialize(PORTC_INDEX,0xF0);
    ret = gpio_port_direction_intialize(PORTD_INDEX,0xC0);
    
    
}

